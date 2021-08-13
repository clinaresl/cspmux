// -*- coding: utf-8 -*-
// MUXsstack_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 02:26:08.630195657 (1628814368)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Implements a stack of operations performed during a search which can be
// undone thus restoring the previous state. This is mandatory, e.g., in
// searches implementing backtracking

#include "MUXsstack_t.h"

// inserts a new frame at the top of the stack
void sstack_t::push (const frame_t& frame) {

    // just simply add it
    _sstack.push_back (frame);
}

// execute and remove the frame at the top of the stack. This function
// is the combination of exec and pop indeed
void sstack_t::unwind () {

    // before proceeding make sure there is at least one frame
    if (_sstack.size () == 0) {
        throw runtime_error ("[sstack_t::unwind] Empty stack!");
    }

    // first, execute the frame at the top of the stack, i.e., the last one
    _sstack.back ().exec ();

    // and remove it upon successful termination
    _sstack.pop_back ();
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
