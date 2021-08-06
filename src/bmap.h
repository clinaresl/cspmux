// -*- coding: utf-8 -*-
// bmap.h
// -----------------------------------------------------------------------------
//
// Started on <vie 06-08-2021 00:55:56.900971082 (1628204156)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

// Description:
// implementation of a bit map of any length

#ifndef _BMAP__H_
#define _BMAP__H_

#include<vector>

// constants

using namespace std;

//
// Class definition
class bmap_t {

    private:

        // INVARIANT: A bitmap consists of a vector of bytes (chars) which store
        // information (1 or 0) in individual bits. These bits can be addressed
        // (get/set) individually
        vector<char> _bmap;

        // the length is stored separately
        size_t _length;

    public:

        // The default constructor is strictly forbidden
        bmap_t () = delete;

        // Explicit constructor - given a number of bits to store
        bmap_t (int len) {

            // compute the required number of bytes to store up to len bits.
            // Note that if len is not divisible by the number of bits in a byte
            // more locations are then offered
            _length = len/8 + int (len%8 != 0);

            // create the vector and initialize it to zero
            _bmap = vector<char>(_length, 0);
        }

        // accessors

        // get the value at the i-th bit
        bool get (const int i) const;

        // set the value of the i-th bit
        void set (const int i, const bool value);

        // public services
        size_t size () const {

            // since each byte contains 8 bits ...
            return 8*_length;
        }

        bool operator[](size_t pos) const;
        bool& operator[](size_t pos);
};

#endif // _BMAP__H_

// Local Variables:
// mode:c++
// fill-column:80
// End:
