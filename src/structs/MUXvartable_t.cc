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

// variables are indexed by their name. The following operator returns
// the index of any variable registered in the table. In case
// it does not exist, an exception is thrown
const size_t vartable_t::operator[] (const string& name) const {

    // this operation performs in O(log n) where n is the number of entries in
    // the table
    auto it = _indices.find (name);

    // in case no variable is registered with this name, raise an exception
    if (it == _indices.end ()) {
        throw runtime_error ("[vartable_t::operator[]] name not found");
    }

    // otherwise, return its location
    return it->second;
}

// add a new entry to the table of variables and return its index. The
// only necessary information is the variable itself and the first and
// last indices to the values of its domain
size_t vartable_t::add_entry (const variable_t& variable,
                              const size_t first, const size_t last) {

    // variables are identified by their name which has to be unique. Thus, make
    // sure this variable does not exist in this table
    size_t index = string::npos;
    try {
        index = (*this) [variable.get_name ()];
    } catch (runtime_error e) {

        // if operator[] raised an exception, then this key does not exist and
        // it is safe to proceed
        _table.push_back (_entry_t (variable, first, last));

        // and now add it to the map of names
        _indices[variable.get_name ()] = _table.size () - 1;
    }

    // now, in case this variable already exists, immediately raise an exception
    if (index != string::npos) {
        throw runtime_error ("[vartable_t::add_entry] Duplicated variable");
    }

    // otherwise, return the location where this variable was stored
    return _table.size () - 1;
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

// set the number of plausible values of the i-th variable to the specified
// value. It returns the new number of plausible values of this entry
const size_t vartable_t::set_nbvalues (const size_t i, const size_t value) {

    // first, make sure the index requested is within the size of this table
    if (i < 0 || i >= (int) _table.size ()) {
        throw out_of_range ("[vartable_t::set_nbvalues] out of bounds");
    }

    // in case it is a correct index, then set the number of plausible values.
    // Note that no verification is performed!
    _table[i]._nbvalues = value;

    // and return the new number of plausible values
    return _table[i].get_nbvalues ();
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
