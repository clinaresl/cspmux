// -*- coding: utf-8 -*-
// MUXsstack_t.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 02:19:35.452149773 (1628813975)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//
// Implements a stack of frames (i.e., atomic operations performed during a
// search) which are executed along a path which can be undone thus restoring
// the previous state. This is mandatory, e.g., in searches implementing
// backtracking

#ifndef _MUXSSTACK_T_H_
#define _MUXSSTACK_T_H_

#include<stdexcept>
#include<vector>

#include "MUXframe_t.h"

using namespace std;

// Class definition
//
// Definition of a stack of operations
class sstack_t {

    private:

        // INVARIANT: a stack consists just of a vector of frames which stand
        // for all actions performed along a path during search
        vector<frame_t> _sstack;

    public:

        // Default constructor - stacks are built by default
        sstack_t () :
            _sstack { vector<frame_t>() }
        {}

        // Stacks can not be copy-constructed
        sstack_t (const sstack_t&) = delete;

        // modifiers

        // inserts a new frame in the stack
        void push (const frame_t& frame);

        // execute and remove the frame at the top of the stack. This function
        // is the combination of exec and pop indeed
        void unwind ();

        // capacity

        // return the number of frames in this stack
        size_t size () const {
            return _sstack.size ();
        }
};

#endif // _MUXSSTACK_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
