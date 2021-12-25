/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: calc.hpp

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

#ifndef CALC_HPP
#define CALC_HPP

#include <vector>
#include <memory>

#include "conf.hpp"

class Calc {

public:

    Calc(const std::shared_ptr<Conf> &conf);

    bool calculate();
    bool createReport() const;

private:

    std::shared_ptr<Conf> m_conf;

    double m_tk    = 0;
    double m_tks   = 0;
    double m_pa    = 0;
    double m_gamma = 0;
    double m_ta    = 0;
    double m_l0s   = 0;
    double m_l0    = 0;
    double m_va    = 0;

    std::vector<double> m_phi_comp;
    std::vector<double> m_sigma_comp;
    std::vector<double> m_psialpha_comp;
    std::vector<double> m_v_comp;
    std::vector<double> m_p_comp;
    std::vector<double> m_t_comp;

    std::vector<double> m_phi_fire;
    std::vector<double> m_x_fire;
    std::vector<double> m_w0_fire;
    std::vector<double> m_beta_fire;
    std::vector<double> m_sigma_fire;
    std::vector<double> m_psialpha_fire;
    std::vector<double> m_v_fire;
    std::vector<double> m_k_fire;
    std::vector<double> m_ks_fire;
    std::vector<double> m_p_fire;
    std::vector<double> m_t_fire;

    std::vector<double> m_phi_exp;
    std::vector<double> m_sigma_exp;
    std::vector<double> m_psialpha_exp;
    std::vector<double> m_v_exp;
    std::vector<double> m_p_exp;
    std::vector<double> m_t_exp;

    double m_li = 0;
    double m_pi = 0;
    double m_etai = 0;
    double m_gi = 0;

    double m_pm = 0;
    double m_pe = 0;
    double m_etam = 0;
    double m_etae = 0;
    double m_ge = 0;

    double m_Ne = 0;
};

#endif // CALC_HPP
