// -*- coding: utf-8 -*-
// TSThelpers.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 20:19:37.924825401 (1628360377)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// helper functions for all tests
//

#ifndef _TSTHELPERS_H_
#define _TSTHELPERS_H_

#include<cstdlib>
#include<set>

using namespace std;

// return a set with n numbers at most randomly generated in the interval [0,
// m). If a random number was already generated it is discarded but the count
// increases
set<int> randSetInt (int n, int m);

#endif // _TSTHELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
