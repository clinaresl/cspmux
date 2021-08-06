// -*- coding: utf-8 -*-
// multibmap_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 06-08-2021 17:30:38.411989678 (1628263838)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description:
// implementation of an array of bitmaps
//

#include<stdexcept>

#include "multibmap_t.h"

// get the value of the j-th bit of the i-th entry
bool multibmap_t::get (const size_t i, const size_t j) const {

    // first, make sure the value requested is within the length of the bit map
    if (i < 0 || i >= _multibmap.size ()) {
        throw out_of_range ("[multibmap_t::get] out of bounds");
    }

    // if everything went fine then try to get the requested location
    return _multibmap[i][j];
}

// set the value of the j-th bit of the i-th entry
void multibmap_t::set (const size_t i, const size_t j, const bool value) {

    // first, make sure the value requested is within the length of the bit map
    if (i < 0 || i >= _multibmap.size ()) {
        throw out_of_range ("[multibmap_t::set] out of bounds");
    }

    // if everything went fine then try to set the requested value in the
    // specified location
    _multibmap[i].set (j, value);
}


// Local Variables:
// mode:c++
// fill-column:80
// End:
