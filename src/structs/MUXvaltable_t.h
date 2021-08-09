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

        // INVARIANT: a table of values consists of a vector of them so that the
        // index of each value is indeed its location, so that if the index of a
        // value is known, then it can be retrieved in O (1)
        vector<value_t<T>&> _table;

    public:

        // Default constructor - tables can be created only by default
        valtable_t () :
            _table { vector<value_t<T>&>() }
        {}

        // tables of values can not be copied
        valtable_t (const valtable_t& right) = delete;

        // accessors

        // return the value associated to a particular index
        const value_t<T>& get (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i < 0 || i >= (int) _table.size ()) {
                throw out_of_range ("[valtable_t::get] out of bounds");
            }

            // in case it is a correct index, return the value at that position
            return _table[i];
        }

        // modifiers

        // insert a new value into this table. Note that its unique index
        // corresponds to the location it takes into the table. It returns the
        // index given to this value
        size_t insert (const value_t<T>& value) {
            _table.push_back (value);
            return _table.size() - 1;
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
