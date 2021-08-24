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
        // indices to other values
        vector<vector<size_t>> _multivector;

        // note that the status of each entry is stored separately
        vector<bool> _status;

    public:

        // The default constructor is strictly forbidden
        multivector_t () = delete;

        // Explicit constructor - given the length of the array
        multivector_t (const size_t len) :
            _multivector { vector<vector<size_t>>(len, vector<size_t>()) },
            _status { vector<bool>(len, true) }
        {}

        // accessors

        // the following service is provided solely for testing purposes
        bool find (const size_t i, const size_t value) const {
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

        // return whether two multivectors are identical or not
        bool operator==(const multivector_t& right) const {

            // first and overall, verify they both have the same number of items
            if (_multivector.size () != right.size ()) {
                return false;
            }

            // next we test equality explicitly

            // first, check the vectors separately one by one
            for (auto i = 0 ; i < _multivector.size () ; i++) {

                // check that both multivectors have vectors of the same size at
                // the i-th location
                if (_multivector[i].size () != right.get (i).size ()) {
                    return false;
                }
                for (auto j = 0 ; j < _multivector[i].size () ; j++) {
                    if (_multivector[i][j] != right.get (i)[j]) {
                        return false;
                    }
                }
            }

            // secondly, check the status of all entries
            for (auto i = 0 ; i < _multivector.size () ; i++) {
                if (_status[i] != right.get_status (i)) {
                    return false;
                }
            }

            // at this point, they are both proven to be equal
            return true;
        }

        // Likewise, define whether two multivectors are different
        bool operator!= (const multivector_t& right) const {
            return !((*this) == right);
        }

        // accessors to entries of the multibitmap

        // get the i-th vector
        const vector<size_t>& get (const size_t i) const {
            return _multivector[i];
        }
        const vector<size_t>& operator[] (const size_t i) const {
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
