// -*- coding: utf-8 -*-
// MUXaction_t.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 10:55:48.619660401 (1628844948)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Definition of a single action performed by a search algorithm

#ifndef _MUXACTION_T_H_
#define _MUXACTION_T_H_

#include<stddef.h>

// the following type is suitable for functions that perform actions during a
// search. The first value is an index to a structure, the second value
// represents the previous value and the third one stands for the new value.
typedef void (stackHandler) (size_t, size_t, size_t);

class action_t {

    private:

        // INVARIANT: an atommic action is characterized by a function and three
        // integer arguments: the first is an index to a structure, the second
        // represents the previous value and the third (last) one stands for the
        // new value given. What structures have to be updated is private
        // knowledge of the function stored in the frame
        stackHandler* _func;
        size_t _index;
        size_t _prev;
        size_t _next;

    public:

        // Default constructors are strictly forbidden
        action_t () = delete;

        // Explicit constructor
        action_t (stackHandler* func, const size_t index, const size_t prev, const size_t next) :
            _func { func },
            _index {index },
            _prev { prev },
            _next { next }
        {}

        // default copy and move constructors
        action_t (const action_t&) = default;
        action_t (action_t&&) = default;

        // default copy and move assignments
        action_t& operator=(const action_t&);
        action_t& operator=(const action_t&&);

        // accessors

        // the following function invokes the execution of the action with its
        // parameters
        void exec () const {
            (_func) (_index, _prev, _next);
        }
};

#endif // _MUXACTION_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
