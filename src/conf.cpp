/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: conf.cpp

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

#include "conf.hpp"
#include "const.hpp"
#include "prgid.hpp"
#include "auxf.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using std::cout;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::regex_match;

bool Conf::readConfigFile() {

    ifstream fin(CONFIGFILE);

    if (!fin) {

        cout << WARNMSGBLANK << "Can not open file \""
             << CONFIGFILE << "\" to read!\n";
        cout << WARNMSGBLANK << "I will try to create blank.\n\n";

        if (createBlank()) {
            cout << MSGBLANK << "Blank created. "
                 << "Please edit it and restart calculation.\n\n";
        }
        else {
            cout << ERRORMSGBLANK << "Can not create blank!\n\n";
        }

        return false;
    }

    string s;
    vector<string> elem;

    while (!fin.eof()) {

        getline(fin, s);

        if (regex_match(s, regex(COMMENTREGEX)) || s.empty()) {
            continue;
        }

        splitString(s, elem, PARAMDELIMITER);

        if (elem.size() != 2) {

            s.clear();
            elem.clear();

            continue;
        }

        if      ( elem[0] == "boost" ) { m_boost = stringToBool(elem[1]);   }
        else if ( elem[0] == "n"     ) { m_n     = stringToDouble(elem[1]); }
        else if ( elem[0] == "i"     ) { m_i     = stringToDouble(elem[1]); }
        else if ( elem[0] == "vh"    ) { m_vh    = stringToDouble(elem[1]); }
        else if ( elem[0] == "eps"   ) { m_eps   = stringToDouble(elem[1]); }
        else if ( elem[0] == "r"     ) { m_r     = stringToDouble(elem[1]); }
        else if ( elem[0] == "l"     ) { m_l     = stringToDouble(elem[1]); }
        else if ( elem[0] == "p0"    ) { m_p0    = stringToDouble(elem[1]); }
        else if ( elem[0] == "t0"    ) { m_t0    = stringToDouble(elem[1]); }
        else if ( elem[0] == "muv"   ) { m_muv   = stringToDouble(elem[1]); }
        else if ( elem[0] == "pk"    ) { m_pk    = stringToDouble(elem[1]); }
        else if ( elem[0] == "iceff" ) { m_iceff = stringToDouble(elem[1]); }
        else if ( elem[0] == "nk"    ) { m_nk    = stringToDouble(elem[1]); }
        else if ( elem[0] == "alpha" ) { m_alpha = stringToDouble(elem[1]); }
        else if ( elem[0] == "etav"  ) { m_etav  = stringToDouble(elem[1]); }
        else if ( elem[0] == "pr"    ) { m_pr    = stringToDouble(elem[1]); }
        else if ( elem[0] == "tr"    ) { m_tr    = stringToDouble(elem[1]); }
        else if ( elem[0] == "dt"    ) { m_dt    = stringToDouble(elem[1]); }
        else if ( elem[0] == "C"     ) { m_C     = stringToDouble(elem[1]); }
        else if ( elem[0] == "H"     ) { m_H     = stringToDouble(elem[1]); }
        else if ( elem[0] == "O"     ) { m_O     = stringToDouble(elem[1]); }
        else if ( elem[0] == "hu"    ) { m_hu    = stringToDouble(elem[1]); }
        else if ( elem[0] == "teta"  ) { m_teta  = stringToDouble(elem[1]); }
        else if ( elem[0] == "n1"    ) { m_n1    = stringToDouble(elem[1]); }
        else if ( elem[0] == "n2s"   ) { m_n2s   = stringToDouble(elem[1]); }
        else if ( elem[0] == "phiz"  ) { m_phiz  = stringToDouble(elem[1]); }
        else if ( elem[0] == "ksi"   ) { m_ksi   = stringToDouble(elem[1]); }
        else if ( elem[0] == "m"     ) { m_m     = stringToDouble(elem[1]); }
        else if ( elem[0] == "da"    ) { m_da    = stringToDouble(elem[1]); }

        s.clear();
        elem.clear();
    }

    fin.close();

    return true;
}

bool Conf::createBlank() const {

    ofstream fout(CONFIGFILE);

    if (!fout) {
        cout << ERRORMSGBLANK << "Can not open file \"" << CONFIGFILE << "\" to write!\n";
        return false;
    }

    fout << "//\n"
         << "// This is " << PRGNAME << " configuration file.\n"
         << "// Parameter-Value delimiter is symbol \"" << PARAMDELIMITER << "\".\n"
         << "// Vector elements delimiter is symbol \"" << ELEMDELIMITER << "\".\n"
         << "// Table data is entered line by line.\n"
         << "// Text after \"//\" is comment.\n" << "//\n\n";

    fout << "/////// параметры конструкции двигателя\n\n// наличие наддува (0 - нет, 1 - да)\nboost=1\n\n// частота вращения коленчатого вала, об/мин\nn=2200\n\n// количество цилиндров\ni=4\n\n// рабочий объем двигателя, л\nvh=5.1\n\n// степень сжатия\neps=17.5\n\n// радиус кривошипа, мм\nr=67.5\n\n// длина шатуна, мм\nl=209.5\n\n/////// параметры окружающей среды\n\n// барометрическое давление, кПа\np0=101\n\n// температура окружающей среды, грЦ\nt0=25\n\n// кажущийся молекулярный вес воздуха\nmuv=28.95\n\n/////// параметры процесса\n\n// давление наддува, кПа\npk=193\n\n// эффективность ОНВ\niceff=0.88\n\n// показатель адиабаты сжатия в компрессоре\nnk=2.8\n\n// коэффициент избытка воздуха\nalpha=1.4\n\n// коэффициент наполнения\netav=0.9\n\n// давление остаточных газов, кПа\npr=120\n\n// температура остаточных газов, грЦ\ntr=680\n\n// подогрев свежего заряда от стенок, грЦ\ndt=10\n\n/////// параметры топлива\n\n// содержание углерода в топливе (по массе)\nC=0.86\n\n// содержание водорода в топливе (по массе)\nH=0.13\n\n// содержание кислорода в топливе (по массе)\nO=0.01\n\n// низшая теплота сгорания дизельного топлива, ккал/кг\nhu=10140\n\n/////// регулировочные параметры\n\n// угол опережения воспламенения, грПКВ\nteta=14\n\n/////// параметры цикла\n\n// показатель политропы сжатия (1.32-1.38)\nn1=1.38\n\n// показатель политропы расширения (1.35-1.45)\nn2s=1.45\n\n/////// параметры модели\n\n// условная продолжительность сгорания, грПКВ\nphiz=60\n\n// коэффициент эффективности сгорания\nksi=0.87\n\n// показатель характера сгорания\nm=0.6\n\n// шаг расчета грПКВ\nda=1\n";

    fout.close();

    return true;
}
