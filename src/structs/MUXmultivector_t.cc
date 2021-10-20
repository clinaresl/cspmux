// -*- coding: utf-8 -*-
// MUXmultivector.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-08-2021 12:29:52.464978848 (1628764192)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Implementation of an array of vectors

#include "MUXmultivector_t.h"

// return true whether two multivectors are identical or not. This service is
// provided for testing purposes
bool multivector_t::operator==(const multivector_t& right) const {

    // first and overall, verify they both have the same number of items
    if (_mutex.size () != right.size ()) {
        return false;
    }

    // next we test equality explicitly

    // first, check the vectors separately one by one
    for (auto i = 0 ; i < _mutex.size () ; i++) {

        // check that both multivectors have vectors of the same size at
        // the i-th location
        if (_mutex[i].size () != right[i].size ()) {
            return false;
        }

        // verify also the contents. Note that items are expected to be in
        // precisely the same order in both multivectors
        for (auto j = 0 ; j < _mutex[i].size () ; j++) {
            if (_mutex[i][j] != right[i][j]) {
                return false;
            }
        }
    }

    // at this point, they are both proven to be equal
    return true;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
