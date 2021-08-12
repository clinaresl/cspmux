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

#include "MUXbmap_t.h"

using namespace std;

//
// Class definition
class multibmap_t {

    private:

        // INVARIANT: A multimap consists of an array of bitmaps which are
        // stored as a vector bitmaps, some of which might be anbled and others
        // might be disabled
        vector<bmap_t> _multibmap;

        // note that the status of each entry is stored separately. True, these
        // data could be stored in a bitmap but a vector<bool> will be much
        // faster and memory usage will grow only slightly
        vector<bool> _status;

    public:

        // The default constructor is strictly forbidden
        multibmap_t () = delete;

        // Explicit constructor - given the length of the array and the number
        // of bits in each entry, all of which are initially enabled
        multibmap_t (const size_t len, const size_t nbbits) :
            _multibmap { vector<bmap_t>(len, bmap_t (nbbits)) },
            _status { vector<bool>(len, true) }
        {}

        // accessors

        // accessors to the status vector
        bool get_status (const size_t i) const {
            return _status[i];
        }
        void set_status (const size_t i, const bool value) {
            _status[i] = value;
        }

        // accessors to entries of the multibitmap

        // get the i-th vector. This can be used to simulate 2-dimensional
        // access to the underlying bits
        const bmap_t& operator[] (const size_t i) const {
            return _multibmap[i];
        }

        // get the value of the j-th bit of the i-th entry
        bool get (const size_t i, const size_t j) const;

        // set the value of the j-th bit of the i-th entry
        void set (const size_t i, const size_t j, const bool value);

        // return the number of entries of this multibitmap
        size_t size () const {
            return _multibmap.size ();
        }
};

#endif // _MULTIBMAP_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
