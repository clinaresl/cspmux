// -*- coding: utf-8 -*-
// MUXframe_t.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 11:01:24.592113768 (1628845284)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// A frame is defined as a container of actions performed by a search algorithm

#ifndef _MUXFRAME_T_H_
#define _MUXFRAME_T_H_

#include<stdexcept>

#include "MUXaction_t.h"

#include<vector>

using namespace std;

// Class definition
//
// Definition of a container of atomic actions
class frame_t {

    private:

        // INVARIANT: a frame consists just of a vector of actions which stand
        // for all actions performed in a single step of a search algorithm
        vector<action_t> _frame;

    public:

        // Default constructor - frames are built by default
        frame_t () :
            _frame { vector<action_t>() }
        {}

        // accessors

        // the following function invokes the execution of all actions in this
        // frame. Note the execution does not remove any action in it
        void exec () const;

        // modifiers

        // inserts a new action in the frame
        void push (const action_t& action);

        // capacity

        // return the number of actions in this frame
        const size_t size () const {
            return _frame.size ();
        }
};

#endif // _MUXFRAME_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
