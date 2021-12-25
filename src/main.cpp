/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: main.cpp

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

/*
  Программа для теплового расчета четырехтактных дизельных двигателей
  внутреннего сгорания по модели И.И.Вибе, изложенной в методическом пособии
  "Тепловой расчет двигателей внутреннего сгорания", выпущенном
  Челябинским политехническим институтом им. Ленинского комсомола в 1972 году.
*/

#include <iostream>
#include <memory>

#include "prgid.hpp"
#include "const.hpp"
#include "conf.hpp"
#include "calc.hpp"

using std::unique_ptr;
using std::shared_ptr;
using std::cout;
using std::cin;

int main(int argc, char **argv) {

    cout << "\n\t" << PRGNAME << " v" << PRGVERSION << "\n"
         << "\t" << PRGDESCRIPTION << "\n\n"
         << "Copyright (C) " << PRGCOPYRIGHTYEARS << " " << PRGAUTHORS << "\n"
         //<< "Source code hosting: " << PRGSOURCECODEHOSTING << "\n"
         << "Author's blog (RU): " << PRGAUTHORSBLOG << "\n\n"
         << PRGLICENSEINFORMATION << "\n\n";

    bool start = true;

    shared_ptr<Conf> conf(new Conf());

    if (!conf->readConfigFile()) {
        cout << ERRORMSGBLANK << "Calculation failed!\n";
        start = false;
    }

    if (start) {
        unique_ptr<Calc> calc(new Calc(conf));
        if (calc->calculate()) {
            calc->createReport();
        }
        else {
            cout << ERRORMSGBLANK << "Calculation failed!\n";
        }
    }

    cout << "\n\nPress Enter to exit...";
    cin.get();

    return 0;
}
