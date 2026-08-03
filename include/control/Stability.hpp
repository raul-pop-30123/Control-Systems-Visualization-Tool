#ifndef STABILITY_HPP
#define STABILITY_HPP

#include <vector>
#include "math/Polynomial.hpp"

// Pc(s) = a_n*s^n + ... + a_1*s + a_0
// R-H table
// s^n          an, an-2, an-4 ...
// s^n-1        an-1, an-3, an-5 ...
// s^n-2        bn bn-1 ....



// bn = det an   an-2         bn-1 = det an   an-4   
//          an-1 an-3                    an-1 an-5
//     ---------------              ---------------
//          an-1                         an-1

struct RouthResult {
    using vect = std::vector<double>;
    std::vector<vect> table;
    int rightHalfPlaneRoots;
    bool stable;
    bool marginallyStable;
};

class Stability {
public:
    static RouthResult routhHurwitz(const Polynomial& p);
};

#endif
