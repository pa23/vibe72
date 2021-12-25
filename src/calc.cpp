/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: calc.cpp

  Copyright (C) 2021 Artem Petrov <pa23666@yandex.ru>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "calc.hpp"
#include "const.hpp"
#include "prgid.hpp"
#include "conf.hpp"
#include "auxf.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <iomanip>

using std::cout;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;
using std::istreambuf_iterator;
using std::setw;
using std::setprecision;
using std::fixed;
using std::setfill;

Calc::Calc(const shared_ptr<Conf> &conf) {
    m_conf = conf;
}

bool Calc::calculate() {

    const bool   c_boost = m_conf->val_boost();
    const double c_n     = m_conf->val_n();
    const double c_i     = m_conf->val_i();
    const double c_vh    = m_conf->val_vh();
    const double c_eps   = m_conf->val_eps();
    const double c_r     = m_conf->val_r();
    const double c_l     = m_conf->val_l();

    const double c_p0    = m_conf->val_p0();
    const double c_t0    = m_conf->val_t0();
    const double c_muv   = m_conf->val_muv();

    const double c_pk    = m_conf->val_pk();
    const double c_iceff = m_conf->val_iceff();
    const double c_nk    = m_conf->val_nk();
    const double c_alpha = m_conf->val_alpha();
    const double c_etav  = m_conf->val_etav();
    const double c_pr    = m_conf->val_pr();
    const double c_tr    = m_conf->val_tr();
    const double c_dt    = m_conf->val_dt();

    const double c_C     = m_conf->val_C();
    const double c_H     = m_conf->val_H();
    const double c_O     = m_conf->val_O();
    const double c_hu    = m_conf->val_hu();

    const double c_teta  = m_conf->val_teta();

    const double c_n1    = m_conf->val_n1();
    const double c_n2s   = m_conf->val_n2s();

    const double c_phiz  = m_conf->val_phiz();
    const double c_ksi   = m_conf->val_ksi();
    const double c_m     = m_conf->val_m();
    const double c_da    = m_conf->val_da();

    // inlet phase calculation

    if (c_boost) {
        m_tk = pow(c_pk / c_p0, (c_nk - 1.0) / c_nk) * (c_t0 + 273.0);
        m_tks = m_tk - c_iceff * (m_tk - (c_t0 + 273.0));
        m_pa = ((c_eps - 1.0) * c_etav * kpa_to_kgfcm2(c_pk) * (m_tks + c_dt) / m_tks + kpa_to_kgfcm2(c_pr)) / c_eps;
        m_gamma = (1.0 / (c_eps - 1.0) / c_etav) * (c_pr / c_pk) * (m_tks / c_tr);
        m_ta = (m_tks + c_dt + m_gamma * c_tr) / (1.0 + m_gamma);
    }
    else {
        m_pa = ((c_eps - 1.0) * c_etav * kpa_to_kgfcm2(c_p0) * (c_t0 + c_dt) / c_t0 + kpa_to_kgfcm2(c_pr)) / c_eps;
        m_gamma = (1.0 / (c_eps - 1.0) / c_etav) * (c_pr / c_p0) * (c_t0 / c_tr);
        m_ta = (c_t0 + c_dt + m_gamma * c_tr) / (1.0 + m_gamma);
    }

    m_l0s = (8.0 / 3.0 * c_C + 8.0 * c_H - c_O) / 0.232;
    m_l0 = (c_C / 12.0 + c_H / 4.0 - c_O / 32.0) / 0.21;
    m_va = (848.0 / 10000.0 / c_muv) * (m_ta / m_pa);

    // compression phase calculation

    double phi = -180;

    const double lam = c_r / c_l;

    while (phi <= -c_teta) {

        m_phi_comp.push_back(phi);

        const double phi_rad = phi * PI / 180.0;
        const double sigma = (1.0 + 1.0 / lam) - (cos(phi_rad) + 1.0 / lam * pow(1.0 - pow(lam, 2.0) * pow(sin(phi_rad), 2.0), 0.5));
        m_sigma_comp.push_back(sigma);

        const double psialpha = 1.0 + (c_eps - 1.0) / 2.0 * sigma;
        m_psialpha_comp.push_back(psialpha);

        const double v = m_va / c_eps * psialpha;
        m_v_comp.push_back(v);

        const double p = m_pa * pow(m_va / v, c_n1);
        m_p_comp.push_back(p);

        const double t = m_ta * pow(m_va / v, c_n1 - 1.0);
        m_t_comp.push_back(t);

        phi += c_da;
    }

    // fire phase calculation

    const double qz = c_ksi * c_hu / (1.0 + m_gamma) / c_alpha / m_l0s;
    const double beta0max = 1.0 + (c_H / 4.0 + c_O / 32.0) / c_alpha / m_l0;
    const double betamax = (beta0max + m_gamma) / (1.0 + m_gamma);

    const double phi_fire_max = c_phiz - c_teta;

    phi -= c_da;
    const double phi_start = phi;
    const size_t vector_size = c_phiz / c_da + 1;

    m_phi_fire.resize(vector_size, 0);
    m_x_fire.resize(vector_size, 0);
    m_w0_fire.resize(vector_size, 0);
    m_beta_fire.resize(vector_size, 0);
    m_sigma_fire.resize(vector_size, 0);
    m_psialpha_fire.resize(vector_size, 0);
    m_v_fire.resize(vector_size, 0);
    m_k_fire.resize(vector_size, 0);
    m_ks_fire.resize(vector_size, 0);
    m_p_fire.resize(vector_size, 0);
    m_t_fire.resize(vector_size, 0);

    const double py = m_p_comp[m_p_comp.size()-1];
    const double ty = m_t_comp[m_t_comp.size()-1];
    const double k_ty = ty / py / m_psialpha_comp[m_psialpha_comp.size()-1];

    for (size_t i=0; i<vector_size; i++) {

        m_phi_fire[i] = phi;

        const double phi_rel = phi + c_teta;

        m_x_fire[i] = 1.0 - pow(E, -6.908 * pow(phi_rel / c_phiz, c_m + 1.0));
        m_w0_fire[i] = 6.908 * (c_m + 1.0) * pow(phi_rel / c_phiz, c_m) * pow(E, -6.908 * pow(phi_rel / c_phiz, c_m + 1.0));
        m_beta_fire[i] = 1.0 + (betamax - 1.0) * m_x_fire[i];

        const double phi_rad = phi * PI / 180.0;

        m_sigma_fire[i] = (1.0 + 1.0 / lam) - (cos(phi_rad) + 1.0 / lam * pow(1.0 - pow(lam, 2.0) * pow(sin(phi_rad), 2.0), 0.5));
        m_psialpha_fire[i] = 1.0 + (c_eps - 1.0) / 2.0 * m_sigma_fire[i];
        m_v_fire[i] = m_va / c_eps * m_psialpha_fire[i];

        if (i == 0) {
            m_k_fire[i] = 1.259 + 76.7 / ty - (0.005 + 0.0372 / c_alpha) * m_x_fire[i];
            m_ks_fire[i] = (m_k_fire[i] + 1.0) / (m_k_fire[i] - 1.0);
            m_p_fire[i] = py;
            m_t_fire[i] = ty;
            phi += c_da;
            continue;
        }

        m_k_fire[i] = 1.259 + 76.7 / m_t_fire[i-1] - (0.005 + 0.0372 / c_alpha) * ((m_x_fire[i-1] + m_x_fire[i]) / 2.0);

        const double ks = (m_k_fire[i-1] + m_k_fire[i]) / 2.0;
        m_ks_fire[i] = (ks + 1.0) / (ks - 1.0);

        m_p_fire[i] = (0.0854 * c_eps / m_va * qz * (m_x_fire[i] - m_x_fire[i-1]) + m_p_fire[i-1] * (m_ks_fire[i] * m_psialpha_fire[i-1] - m_psialpha_fire[i])) / (m_ks_fire[i] * m_psialpha_fire[i] - m_psialpha_fire[i-1]);
        m_t_fire[i] = k_ty * m_p_fire[i] * m_psialpha_fire[i] / ((m_beta_fire[i-1] + m_beta_fire[i]) / 2.0);

        //

        phi += c_da;
    }

    // expansion phase calculation

    phi = m_phi_fire[m_phi_fire.size()-1] + c_da;
    const double pz = m_p_fire[m_p_fire.size()-1];
    const double tz = m_t_fire[m_t_fire.size()-1];
    const double vz = m_v_fire[m_v_fire.size()-1];

    while (phi <= 180.0) {

        m_phi_exp.push_back(phi);

        const double phi_rad = phi * PI / 180.0;
        const double sigma = (1.0 + 1.0 / lam) - (cos(phi_rad) + 1.0 / lam * pow(1.0 - pow(lam, 2.0) * pow(sin(phi_rad), 2.0), 0.5));
        m_sigma_exp.push_back(sigma);

        const double psialpha = 1.0 + (c_eps - 1.0) / 2.0 * sigma;
        m_psialpha_exp.push_back(psialpha);

        const double v = m_va / c_eps * psialpha;
        m_v_exp.push_back(v);

        const double p = pz * pow(vz / v, c_n2s);
        m_p_exp.push_back(p);

        const double t = tz * pow(vz / v, c_n2s - 1.0);
        m_t_exp.push_back(t);

        phi += c_da;
    }

    // indicated parameters calculation

    const double lay = 10000.0 * m_va / (c_eps * (c_n1 - 1.0)) * (m_pa * c_eps - py * m_psialpha_comp[m_psialpha_comp.size()-1]);
    const double lzb = 10000.0 * m_va / (c_eps * (c_n2s - 1.0)) * (pz * m_psialpha_fire[m_psialpha_fire.size()-1] - m_p_exp[m_p_exp.size()-1] * c_eps);

    double sum = 0;

    for (size_t i=1; i<m_p_fire.size(); i++) {
        sum += ((m_p_fire[i-1] + m_p_fire[i]) / 2.0) * (m_psialpha_fire[i] - m_psialpha_fire[i-1]);
    }

    const double lyz = 10000.0 * m_va / c_eps * sum;

    m_li = lay + lzb + lyz;
    m_pi = c_eps / 10000.0 / (c_eps - 1.0) * m_li / m_va;
    m_etai = c_ksi * m_li / 427 / qz;
    m_gi = 1000.0 * 632 / c_hu / m_etai;

    // effective parameters calculation

    const double cm = c_r * 0.001 * 2.0 * c_n / 30.0;

    double a = 0;
    double b = 0;

    if (c_i <= 6) {
        a = 0.9;
        b = 0.12;
    }
    else if (c_i <= 8 ) {
        a = 0.7;
        b = 0.12;
    }
    else {
        a = 0.3;
        b = 0.12;
    }

    m_pm = a + b * cm;
    m_pe = m_pi - m_pm;
    m_etam = m_pe / m_pi;
    m_etae = m_etai * m_etam;
    m_ge = 1000.0 * 632 / c_hu / m_etae;

    m_Ne = m_pe * c_vh * c_n / 225.0 / 4.0;

    //

    return true;
}

bool Calc::createReport() const {

    string programName(PRGNAME);
    string dateTime = currDateTime();
    string srcDataFilename = programName + "_source-data_" + dateTime + ".txt";
    string reportFilename = programName + "_results_" + dateTime + ".csv";

    //

    ifstream fin_srcdata(CONFIGFILE);
    string srcdata;

    if (!fin_srcdata) {
        cout << WARNMSGBLANK << "Can not open file \""
             << CONFIGFILE << "\" to read!\n";
        cout << WARNMSGBLANK << "Copying source data will be skipping.\n\n";
    }
    else {
        srcdata.assign((istreambuf_iterator<char>(fin_srcdata)),
                       (istreambuf_iterator<char>()));
    }

    fin_srcdata.close();

    ofstream fout_srccopy(srcDataFilename);

    if (!fout_srccopy) {
        cout << WARNMSGBLANK << "Can not open file \""
             << srcDataFilename << "\" to write!\n";
        cout << WARNMSGBLANK << "Copying source data will be skipping.\n\n";
    }
    else {
        fout_srccopy << srcdata;
    }

    fout_srccopy.close();

    cout << MSGBLANK << "Source data copied to \""
         << srcDataFilename << "\".\n";

    //

    ofstream fout(reportFilename);

    if (!fout) {
        cout << ERRORMSGBLANK << "Can not open file \""
             << reportFilename << "\" to write!\n";
        return false;
    }

    fout << PRGNAME << "\nv" << PRGVERSION << "\n\n";

    fout << "Results of INLET phase calculation\n\n";
    fout << "Tk    = " << fixed << setprecision(1) << m_tk  - 273.0        << " degC\n";
    fout << "Tks   = " << fixed << setprecision(1) << m_tks - 273.0        << " degC\n";
    fout << "Pa    = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_pa)  << " kPa\n";
    fout << "gamma = " << fixed << setprecision(4) << m_gamma              << "\n";
    fout << "Ta    = " << fixed << setprecision(1) << m_ta - 273.0         << " degC\n";
    fout << "L0s   = " << fixed << setprecision(3) << m_l0s                << " kg/kg\n";
    fout << "L0    = " << fixed << setprecision(3) << m_l0                 << " kgmol/kg\n";
    fout << "va    = " << fixed << setprecision(3) << m_va                 << " m3/kg\n\n";

    fout << "Results of COMPRESSION phase calculation\n\n";
    fout << setw(10) << "phi[deg]"
         << setw(10) << "sigma"
         << setw(10) << "psialpha"
         << setw(10) << "v[m3/kg]"
         << setw(10) << "p[kPa]"
         << setw(10) << "t[degC]\n";
    fout << setfill('-') << setw(60);
    fout << "\n";
    fout << setfill(' ');

    for (size_t i=0; i<m_phi_comp.size()-1; i++) {
        fout << setw(10) << setprecision(1) << m_phi_comp[i]
             << setw(10) << setprecision(4) << m_sigma_comp[i]
             << setw(10) << setprecision(4) << m_psialpha_comp[i]
             << setw(10) << setprecision(4) << m_v_comp[i]
             << setw(10) << setprecision(1) << kgfcm2_to_kpa(m_p_comp[i])
             << setw(9)  << setprecision(1) << m_t_comp[i] - 273.0;
        fout << "\n";
    }

    fout << "\n";

    fout << "Results of FIRE phase calculation\n\n";
    fout << setw(10) << "phi[deg]"
         << setw(10) << "x"
         << setw(10) << "w0"
         << setw(10) << "beta"
         << setw(10) << "sigma"
         << setw(10) << "psialpha"
         << setw(10) << "v[m3/kg]"
         << setw(10) << "K"
         << setw(10) << "Ks"
         << setw(10) << "p[kPa]"
         << setw(10) << "t[degC]\n";
    fout << setfill('-') << setw(110);
    fout << "\n";
    fout << setfill(' ');

    for (size_t i=0; i<m_phi_fire.size(); i++) {
        fout << setw(10) << setprecision(1) << m_phi_fire[i]
             << setw(10) << setprecision(4) << m_x_fire[i]
             << setw(10) << setprecision(4) << m_w0_fire[i]
             << setw(10) << setprecision(3) << m_beta_fire[i]
             << setw(10) << setprecision(4) << m_sigma_fire[i]
             << setw(10) << setprecision(4) << m_psialpha_fire[i]
             << setw(10) << setprecision(4) << m_v_fire[i]
             << setw(10) << setprecision(3) << m_k_fire[i]
             << setw(10) << setprecision(3) << m_ks_fire[i]
             << setw(10) << setprecision(1) << kgfcm2_to_kpa(m_p_fire[i])
             << setw(9)  << setprecision(1) << m_t_fire[i] - 273.0;
        fout << "\n";
    }

    fout << "\n";

    fout << "Results of EXPANSION phase calculation\n\n";
    fout << setw(10) << "phi[deg]"
         << setw(10) << "sigma"
         << setw(10) << "psialpha"
         << setw(10) << "v[m3/kg]"
         << setw(10) << "p[kPa]"
         << setw(10) << "t[degC]\n";
    fout << setfill('-') << setw(60);
    fout << "\n";
    fout << setfill(' ');

    for (size_t i=0; i<m_phi_exp.size(); i++) {
        fout << setw(10) << setprecision(1) << m_phi_exp[i]
             << setw(10) << setprecision(4) << m_sigma_exp[i]
             << setw(10) << setprecision(4) << m_psialpha_exp[i]
             << setw(10) << setprecision(4) << m_v_exp[i]
             << setw(10) << setprecision(1) << kgfcm2_to_kpa(m_p_exp[i])
             << setw(9)  << setprecision(1) << m_t_exp[i] - 273.0;
        fout << "\n";
    }

    fout << "\n";

    fout << "Cylinder pressure\n\n";
    fout << "P_comp_max = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_p_fire[0]) << " kPa\n";
    fout << "P_fire_max = " << fixed << setprecision(1) << kgfcm2_to_kpa(maxValue(m_p_fire)) << " kPa\n\n";

    fout << "INDICATED parameters\n\n";
    fout << "li   = " << fixed << setprecision(1) << kgfmkg_to_j(m_li)   << " J\n";
    fout << "pi   = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_pi) << " kPa\n";
    fout << "etai = " << fixed << setprecision(3) << m_etai              << "\n";
    fout << "gi   = " << fixed << setprecision(1) << m_gi * 1.36         << " g/kWh\n\n";

    fout << "EFFECTIVE parameters\n\n";
    fout << "pe   = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_pe) << " kPa\n";
    fout << "etae = " << fixed << setprecision(3) << m_etae              << "\n";
    fout << "Ne   = " << fixed << setprecision(1) << m_Ne / 1.36         << " kW\n";
    fout << "ge   = " << fixed << setprecision(1) << m_ge * 1.36         << " g/kWh\n\n";

    fout.close();

    cout << MSGBLANK << "Report file \"" << reportFilename << "\"created.\n\n";

    //

    cout << "Cylinder pressure\n\n";
    cout << "P_comp_max = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_p_fire[0]) << " kPa\n";
    cout << "P_fire_max = " << fixed << setprecision(1) << kgfcm2_to_kpa(maxValue(m_p_fire)) << " kPa\n\n";

    cout << "EFFECTIVE parameters\n\n";
    cout << "pe   = " << fixed << setprecision(1) << kgfcm2_to_kpa(m_pe) << " kPa\n";
    cout << "etae = " << fixed << setprecision(3) << m_etae              << "\n";
    cout << "Ne   = " << fixed << setprecision(1) << m_Ne / 1.36         << " kW\n";
    cout << "ge   = " << fixed << setprecision(1) << m_ge * 1.36         << " g/kWh\n\n";

    return true;
}
