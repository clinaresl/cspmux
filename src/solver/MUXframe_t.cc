// -*- coding: utf-8 -*-
// MUXframe_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 11:08:49.534929940 (1628845729)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// A frame is defined as a container of actions performed by a search algorithm

#include "MUXframe_t.h"

// the following function invokes the execution of all actions in this
// frame. Note the execution does not remove any action in it
void frame_t::exec () const {

    // Importantly, actions are executed in reversed order
    for (auto it = _frame.rbegin () ; it != _frame.rend () ; ++it ) {

        // invoke the execution of this action
        it->exec ();
    }
}

// inserts a new action in the frame
void frame_t::push (const action_t& action) {

    // just simply add it
    _frame.push_back (action);
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
