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

// Generate a random string with characters in the sequence ASCII(32) -
// ASCII(126) which do not appear in the string exclude
string randString (int length, string exclude)
{

  // just randomly sample a character from the set until the string gets the
  // requested length
  char randchr;
  string randstr = "";
  for (int i = 0 ; i < length ; i++) {
    do {
      randchr = (char) (rand () % (126 - 32 + 1) + 32);
    } while (exclude.find (randchr) != string::npos);
    randstr += randchr;
  }

  return randstr;
}

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

// return a vector with n numbers randomly generated in the interval [0, m)
vector<int> randVectorInt (int n, int m) {

    // create an empty vector
    vector<int> result;

    // generate n random numbers
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random number
        int item = rand () % m;

        // and insert it into the vector
        result.push_back (item);
    }

    // and return the vector
    return result;
}

// return a vector with n strings randomly generated in the interval [0, m),
// each with l chars
vector<string> randVectorString (int n, int m, int l) {

    // create an empty vector
    vector<string> result;

    // generate n random strings
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random string
        string item = randString (l);

        // and insert it into the vector
        result.push_back (item);
    }

    // and return the vector
    return result;
}

// return a vector with n times randomly generated in the interval [0, m), where
// m is measured as the number of seconds elapsed since epoch
vector<time_t> randVectorTime (int n, long long int m) {

    // create an empty vector
    vector<time_t> result;

    // generate n random timings
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random time
        time_t item = time_t(rand () % m);

        // and insert it into the vector
        result.push_back (item);
    }

    // and return the vector
    return result;
}

// return a vector of n pairs of ints: the first element is randomly chosen in
// the interval [0, m), and the second is randomly chosen in the interval [m,
// m+delta) or [m-delta, m) depending on whether delta is either positive or
// negative
vector<pair<int, int>> randVectorIntPair (int n, int m, int delta) {

    // create an empty vector
    vector<pair<int,int>> result;

    // generate n random pairs of ints
    for (auto i = 0 ; i < n ; i++) {

        // generate the first random number
        int item1 = rand () % m;

        // and now generate the second one. Note that if delta is zero then the
        // second number is necessarily equal to the first one
        int item2;
        if (delta > 0) {
            item2 = item1 + rand () % delta;
        } else if (delta < 0) {
            item2 = item1 - rand () % (-delta);
        } else {
            item2 = item1;
        }

        // and insert it into the vector
        result.push_back (pair<int, int>{item1, item2});
    }

    // and return the vector
    return result;
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
