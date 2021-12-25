/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: auxf.cpp

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

#include <string>
#include <vector>
#include <sstream>
#include <ctime>

#include "auxf.hpp"

using std::string;
using std::vector;
using std::ostringstream;
using std::istringstream;

string uintToString(size_t x) {

    ostringstream stm;
    stm << x;

    return stm.str();
}

double stringToDouble(const string &str) {

    istringstream stm;
    double val = 0;

    stm.str(str);
    stm >> val;

    return val;
}

bool stringToBool(const string &str) {

    istringstream stm;
    bool val = 0;

    stm.str(str);
    stm >> val;

    return val;
}

void splitString(
    const string &fullstr,
    vector<string> &elements,
    const string &delimiter
    ) {

    string::size_type lastpos = fullstr.find_first_not_of(delimiter, 0);
    string::size_type pos = fullstr.find_first_of(delimiter, lastpos);

    while ( (string::npos != pos) || (string::npos != lastpos) ) {

        elements.push_back(fullstr.substr(lastpos, pos-lastpos));

        lastpos = fullstr.find_first_not_of(delimiter, pos);
        pos = fullstr.find_first_of(delimiter, lastpos);
    }
}

string currDateTime() {

    time_t t = time(NULL);
    struct tm *dtnow = localtime(&t);

    const string year = uintToString(dtnow->tm_year + 1900);
    const string mon  = uintToString(dtnow->tm_mon + 1);
    const string day  = uintToString(dtnow->tm_mday);
    const string hour = uintToString(dtnow->tm_hour);
    const string min  = uintToString(dtnow->tm_min);

    return
        year + "-" + trimDate(mon) + "-" + trimDate(day) + "_" +
        trimDate(hour) + "-" + trimDate(min);
}

string trimDate(const string &str) {

    if (str.size() == 1) {
        return "0" + str;
    }
    else if (str.size() == 2) {
        return str;
    }

    return "00";
}

double kpa_to_kgfcm2(double kpa) {
    return
        kpa * 0.010197162;
}

double kgfcm2_to_kpa(double kgfcm2) {
    return
        kgfcm2 / 0.010197162;
}

double kgfmkg_to_j(double kgfmkg) {
    return
        kgfmkg * 0.10197162;
}

double maxValue(const vector<double> &v) {

    double maxv = v[0];

    for (size_t i=1; i<v.size(); i++) {
        if (v[i] > maxv) {
            maxv = v[i];
        }
    }

    return maxv;
}
