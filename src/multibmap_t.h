// -*- coding: utf-8 -*-
// bmap.h
// -----------------------------------------------------------------------------
//
// Started on <vie 06-08-2021 00:55:56.900971082 (1628204156)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

// Description:
// implementation of an array of bitmaps

#ifndef _MULTIBMAP_T_H_
#define _MULTIBMAP_T_H_

#include<vector>

#include "bmap_t.h"

// constants

using namespace std;

//
// Class definition
class multibmap_t {

    private:

        // INVARIANT: A multimap consists of an array of bitmaps which are
        // stored as a vector bitmaps
        vector<bmap_t> _multibmap;

    public:

        // The default constructor is strictly forbidden
        multibmap_t () = delete;

        // Explicit constructor - given the length of the array and the number
        // of bits in each entry
        multibmap_t (const size_t len, const size_t nbbits) :
            _multibmap { vector<bmap_t>(len, bmap_t (nbbits)) }
        {}

        // accessors

        // get the i-th vector. This can be used to simulate 2-dimensional
        // access to the underlying bits
        const bmap_t& operator[] (const size_t i) const {
            return _multibmap[i];
        }

        // get the value of the j-th bit of the i-th entry
        bool get (const size_t i, const size_t j) const;

        // set the value of the j-th bit of the i-th entry
        void set (const size_t i, const size_t j, const bool value);

};

#endif // _MULTIBMAP_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
