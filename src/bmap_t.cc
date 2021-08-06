// -*- coding: utf-8 -*-
// bmap.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 06-08-2021 01:07:24.426741022 (1628204844)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

// Description:
// implementation of a bit map of any length
//
// Note that bits are not stored in contiguous locations as in:
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// <------- byte 0 --------><------- byte 1 ------->
//
// instead bytes are stored in increasing order but the offset is sorted in
// descending order as in:
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | 7| 6| 5| 4| 3| 2| 1| 0|15|14|13|12|11|10| 9| 8|
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// <------- byte 0 --------><------- byte 1 ------->
//

#include<stdexcept>

#include "bmap_t.h"

// get the value at the i-th bit
bool bmap_t::get (const size_t i) const {

    // first, make sure the value requested is within the length of the bit map
    if (i < 0 || i >= (int) size ()) {
        throw out_of_range ("[bmap] out of bounds");
    }

    // get the location of the required bit. It resides in the i-th byte which
    // is computed dividing by 8, and with an offset equal to the remainder of
    // the same division
    int ibyte = i/8;
    int offset = i%8;

    // the return the required content. This is achieved by right shifting the
    // i-th byte as many locations as the offset and then extracting the LSB
    // (less significant bit)
    return _bmap[ibyte] >> offset & 1;
}

// set the value of the i-th bit
void bmap_t::set (const size_t i, const bool value) {

    // first, make sure the value requested is within the length of the bit map
    if (i < 0 || i >= (int) size ()) {
        throw out_of_range ("[bmap] out of bounds");
    }

    // get the location of the required bit. It resides in the i-th byte which
    // is computed dividing by 8, and with an offset equal to the remainder of
    // the same division
    int ibyte = i/8;
    int offset = i%8;

    // to set a bit in one specific location, it just suffices left shifting it
    // its offset and computing the bitwise with the current contents of the
    // bitmap
    if (value) {
        _bmap[ibyte] |= 1 << offset;
    } else {

        // to reset a bit the idea is pretty much the same: performing a bitwise
        // and with a mask made of 1s but the desired location which gets the
        // value 0
        _bmap[ibyte] &= ~(1 << offset);
    }
}


// Local Variables:
// mode:c++
// fill-column:80
// End:
