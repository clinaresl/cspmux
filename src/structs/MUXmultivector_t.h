// -*- coding: utf-8 -*-
// MUXmultivector.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-08-2021 12:06:13.307181515 (1628762773)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Implementation of an array of vectors

#ifndef _MUXMULTIVECTOR_H_
#define _MUXMULTIVECTOR_H_

#include<algorithm>
#include<vector>

using namespace std;

//
// Class definition
class multivector_t {

    private:

        // INVARIANT: A multivector consists of an array of vectors which store
        // integers
        vector<vector<int>> _multivector;

        // note that the status of each entry is stored separately
        vector<bool> _status;

    public:

        // The default constructor is strictly forbidden
        multivector_t () = delete;

        // Explicit constructor - given the length of the array
        multivector_t (const size_t len) :
            _multivector { vector<vector<int>>(len, vector<int>()) },
            _status { vector<bool>(len, true) }
        {}

        // accessors

        // the following service is provided solely for testing purposes
        bool find (const size_t i, const int value) {
            return (std::find (_multivector[i].begin (),
                               _multivector[i].end (),
                               value) != _multivector[i].end ());
        }

        // accessors to the status vectors
        bool get_status (const size_t i) const {
            return _status[i];
        }
        void set_status (const size_t i, const bool value) {
            _status[i] = value;
        }

        // accessors to entries of the multibitmap

        // get the i-th vector
        const vector<int>& operator[] (const size_t i) const {
            return _multivector[i];
        }

        // set the value j in the i-th vector
        void set (const size_t i, const size_t j) {
            _multivector[i].push_back (j);
        }

        // return the number of entries in the multivector
        size_t size () const {
            return _multivector.size ();
        }
};

#endif // _MUXMULTIVECTOR_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
