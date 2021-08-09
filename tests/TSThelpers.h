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
#include<ctime>
#include<set>
#include<string>
#include<vector>

using namespace std;

// Generate a random string with characters in the sequence ASCII(32) -
// ASCII(126) which do not appear in the string exclude
string randString (int length, string exclude="");

// return a set with n numbers at most randomly generated in the interval [0,
// m). If a random number was already generated it is discarded but the count
// increases
set<int> randSetInt (int n, int m);

// return a vector with n numbers randomly generated in the interval [0, m)
vector<int> randVectorInt (int n, int m);

// return a vector with n strings randomly generated in the interval [0, m),
// each with l chars
vector<string> randVectorString (int n, int m, int l=10);

// return a vector with n times randomly generated in the interval [0, m), where
// m is measured as the number of seconds elapsed since epoch
vector<time_t> randVectorTime (int n, long long int m);


#endif // _TSTHELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
