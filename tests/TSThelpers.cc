// -*- coding: utf-8 -*-
// TSThelpers.cc
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 20:22:01.706324991 (1628360521)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// helper functions for all tests
//

#include "TSThelpers.h"

// return a set with n numbers at most randomly generated in the interval [0,
// m). If a random number was already generated it is discarded but the count
// increases
set<int> randSetInt (int n, int m) {

    // create an empty set
    set<int> result;

    // generate n random numbers
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random number
        int item = rand () % m;

        // and insert it into the set
        result.insert (item);
    }

    // and return the set
    return result;
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
