// -*- coding: utf-8 -*-
// MUXvartable_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 16:09:12.376583040 (1628518152)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// A table of CSP variables record the indexes of every variable along with
// information about them

#include "MUXvartable_t.h"

// return the variable in the i-th location of this table
const variable_t& vartable_t::get_variable (const size_t i) const {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::get_variable] out of bounds");
    }

    // in case it is a correct index, return the variable at that position
    return _table[i].get_variable ();
}

// return the index to the first value in the domain of the i-th variable
const size_t vartable_t::get_first (const size_t i) const {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::get_first] out of bounds");
    }

    // in case it is a correct index, return the index to the first value in the
    // domain at that position
    return _table[i].get_first ();
}

// return the index to the last value in the domain of the i-th variable
const size_t vartable_t::get_last (const size_t i) const {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::get_last] out of bounds");
    }

    // in case it is a correct index, return the index to the last value in the
    // domain at that position
    return _table[i].get_last ();
}

// return the number of plausible values in the domain of the i-th
// variable
const size_t vartable_t::get_nbvalues (const size_t i) const {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::get_nbvalues] out of bounds");
    }

    // in case it is a correct index, return the number of plausible values at
    // that position
    return _table[i].get_nbvalues ();
}

// return the index to the value assigned to the i-th variable. If none
// has been assigned yet then -1 is returned
long int vartable_t::get_value (const size_t i) const {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::get_value] out of bounds");
    }

    // in case it is a correct index, return the value at that position
    return _table[i].get_value ();
}

// assign the index of one value in the domain of the i-th variable to
// it
void vartable_t::assign (const size_t i, const long int value) {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::assign] out of bounds");
    }

    // in case it is a correct index, assign the given value to this variable
    _table[i] = value;
}

// decrement the number of plausible values of the i-th variable. It returns the
// new number of plausible values of this entry
const size_t vartable_t::decrement_nbvalues (const size_t i) {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::decrement_nbvalues] out of bounds");
    }

    // in case it is a correct index, decrement the number of plausible values
    // of this variable
    _table[i] -= 1;

    // and return the new number of plausible values
    return _table[i].get_nbvalues ();
}

// increment the number of plausible values of the i-th variable. It returns the
// new number of plausible values of this entry
const size_t vartable_t::increment_nbvalues (const size_t i) {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::increment_nbvalues] out of bounds");
    }

    // in case it is a correct index, increment the number of plausible values
    // of this variable
    _table[i] += 1;

    // and return the new number of plausible values
    return _table[i].get_nbvalues ();
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
