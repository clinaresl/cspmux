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

        // default copy and move constructors
        frame_t (const frame_t&) = default;
        frame_t (frame_t&&) = default;

        // default copy and move assignments
        frame_t& operator=(const frame_t&) = default;
        frame_t& operator=(frame_t&&) = default;

        // accessors

        // the following function invokes the execution of all actions in this
        // frame. Note the execution does not remove any action in it
        void exec () const {

            // Importantly, actions are executed in reversed order
            for (auto it = _frame.rbegin () ; it != _frame.rend () ; ++it) {

                // Invoke the execution of this action
                it->exec ();
            }
        }

        // modifiers

        // inserts a new action in the frame
        frame_t& operator+=(const action_t& action) {
            _frame.push_back (action);
            return *this;
        }

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
