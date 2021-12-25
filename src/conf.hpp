/*
  vibe72
  Termal calculation of four-cycle diesel engines.

  File: conf.hpp

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

#ifndef CONF_HPP
#define CONF_HPP

class Conf {

public:

    bool readConfigFile();

    bool   val_boost() const { return m_boost; }
    double val_n()     const { return m_n;     }
    double val_i()     const { return m_i;     }
    double val_vh()    const { return m_vh;    }
    double val_eps()   const { return m_eps;   }
    double val_r()     const { return m_r;     }
    double val_l()     const { return m_l;     }

    double val_p0()    const { return m_p0;    }
    double val_t0()    const { return m_t0;    }
    double val_muv()   const { return m_muv;   }

    double val_pk()    const { return m_pk;    }
    double val_iceff() const { return m_iceff; }
    double val_nk()    const { return m_nk;    }
    double val_alpha() const { return m_alpha; }
    double val_etav()  const { return m_etav;  }
    double val_pr()    const { return m_pr;    }
    double val_tr()    const { return m_tr;    }
    double val_dt()    const { return m_dt;    }

    double val_C()     const { return m_C;     }
    double val_H()     const { return m_H;     }
    double val_O()     const { return m_O;     }
    double val_hu()    const { return m_hu;    }

    double val_teta()  const { return m_teta;  }

    double val_n1()    const { return m_n1;    }
    double val_n2s()   const { return m_n2s;   }

    double val_phiz()  const { return m_phiz;  }
    double val_ksi()   const { return m_ksi;   }
    double val_m()     const { return m_m;     }
    double val_da()    const { return m_da;    }

private:

    bool createBlank() const;

    bool   m_boost = false;
    double m_n     = 0;
    double m_i     = 0;
    double m_vh    = 0;
    double m_eps   = 0;
    double m_r     = 0;
    double m_l     = 0;

    double m_p0  = 0;
    double m_t0  = 0;
    double m_muv = 0;

    double m_pk    = 0;
    double m_iceff = 0;
    double m_nk    = 0;
    double m_alpha = 0;
    double m_etav  = 0;
    double m_pr    = 0;
    double m_tr    = 0;
    double m_dt    = 0;

    double m_C  = 0;
    double m_H  = 0;
    double m_O  = 0;
    double m_hu = 0;

    double m_teta = 0;

    double m_n1  = 0;
    double m_n2s = 0;

    double m_phiz = 0;
    double m_ksi  = 0;
    double m_m    = 0;
    double m_da   = 0;

};

#endif // CONF_HPP
