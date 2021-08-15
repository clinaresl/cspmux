// -*- coding: utf-8 -*-
// MUXvaltable_t.hh
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 01:33:09.090363933 (1628465589)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// A table of values records the index of every value

#ifndef _VALTABLE_T_H_
#define _VALTABLE_T_H_

#include<stdexcept>
#include<vector>

#include "MUXvalue_t.h"

using namespace std;

// Class definition
//
// Definition of a table of values of any type
template<class T>
class valtable_t {

    private:

        template<class U>
        struct _entry_t {

            // INVARIANT: each entry of the table of values stores the value,
            // and the number of (enabled) mutexes it has with other values. A
            // mutex if enabled, if the mutex is still enabled
            value_t<U> _value;
            size_t _nbvalues;

            // Default constructors of entries are strictly forbidden
            _entry_t<U> () = delete;

        };

        // INVARIANT: a table of values consists of a vector of entries. If the
        // index of a value is known, then it can be retrieved in O (1)
        vector<_entry_t<T>> _table;

    public:

        // Default constructor - tables can be created only by default
        valtable_t () :
            _table { vector<_entry_t<T>>() }
        {}

        // accessors

        // return the number of (feasible or enabled) mutex values associated to
        // a particular index
        const size_t get_nbvalues (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i < 0 || i >= (int) _table.size ()) {
                throw out_of_range ("[valtable_t::get_nbvalues] out of bounds");
            }

            // in case it is a correct index, return the number of mutex values
            // at that position
            return _table[i]._nbvalues;
        }

        // return the value associated to a particular index
        const value_t<T>& get_value (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i < 0 || i >= (int) _table.size ()) {
                throw out_of_range ("[valtable_t::get_value] out of bounds");
            }

            // in case it is a correct index, return the value at that position
            return _table[i]._value;
        }

        // modifiers

        // insert a new value into this table. Note that its unique index
        // corresponds to the location it takes into the table. It returns the
        // index given to this value. The number of mutex values is initialized
        // to zero
        size_t insert (const value_t<T>& value) {
            _table.push_back (_entry_t<T> {value, 0});
            return _table.size() - 1;
        }

        // decrement the number of plausible mutexes of the i-th value. It
        // returns the current number of plausible mutexes of this entry after
        // the update
        const size_t decrement_nbvalues (const size_t i) {
            return --_table[i]._nbvalues;
        }

        // increment the number of plausible mutexes of the i-th value. It
        // returns the current number of plausible mutexes of this entry after
        // the update
        const size_t increment_nbvalues (const size_t i) {
            return ++_table[i]._nbvalues;
        }

        void set_nbvalues (const size_t i, const size_t nbvalues) {
            _table[i]._nbvalues = nbvalues;
        }

        // capacity

        // return the number of values in this table
        size_t size () const {
            return _table.size ();
        }
};

#endif // _VALTABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
