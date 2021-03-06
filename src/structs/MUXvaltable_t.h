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

#include<iostream>
#include<stdexcept>
#include<vector>

#include "MUXvalue_t.h"

// Class definition
//
// Definition of a table of values of any type
template<class T>
class valtable_t {

    private:

        template<class U>
        struct _entry_t {

            // INVARIANT: each entry of the table of values stores the value,
            // the status of each entry, i.e., whether it is still active or
            // not, and the number of active mutexes it still has, i.e., the
            // number of enabled values that are threatening it
            value_t<U> _value;
            bool _status;
            size_t _nbmutexes;

            // Default constructors of entries are strictly forbidden
            _entry_t<U> () = delete;

            // default copy and move constructors
            _entry_t (_entry_t&) = default;
            _entry_t (_entry_t&&) = default;

            // default copy and move assignments
            _entry_t& operator=(_entry_t&) = default;
            _entry_t& operator=(_entry_t&&) = default;

            // return whether two entries are the same or not
            bool operator==(const _entry_t& right) const {
                return _value == right._value &&
                    _status == right._status &&
                    _nbmutexes == right._nbmutexes;
            }

            // likewise, define whether they are different
            bool operator!=(const _entry_t& right) const {
                return _value != right._value ||
                    _status != right._status ||
                    _nbmutexes != right._nbmutexes;
            }
        };

        // INVARIANT: a table of values consists of a vector of entries. If the
        // index of a value is known, then it can be retrieved in O (1)
        std::vector<_entry_t<T>> _table;

    public:

        // Default constructor - tables can be created only by default
        valtable_t () :
            _table { std::vector<_entry_t<T>>() }
        {}

        // accessors

        // return the number of (feasible or enabled) mutex values associated to
        // a particular index
        const size_t get_nbmutexes (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable_t::get_nbmutexes] out of bounds");
            }

            // in case it is a correct index, return the number of mutex values
            // at that position
            return _table[i]._nbmutexes;
        }

        // set the number of active mutexes of the i-th value. Return the number
        // of active mutexes written
        const size_t set_nbmutexes (const size_t i, const size_t nbmutexes) {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable::set_nbmutexes] out of bounds");
            }

            // in case this is a correct operation, increment the number of
            // active mutexes by the given amount
            _table[i]._nbmutexes = nbmutexes;
            return _table[i]._nbmutexes;
        }

        // return the status of the i-th value
        bool get_status (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable_t::get_status] out of bounds");
            }

            // in case it is a correct index, return the status of the i-th
            // value
            return _table[i]._status;
        }

        // set the status of the i-th value. It returns the new status
        bool set_status (const size_t i, const bool status) {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable_t::set_status] out of bounds");
            }

            // otherwise, set the status of the i-th value to the specified
            // status
            _table[i]._status = status;
            return _table[i]._status;
        }

        // return the value associated to a particular index
        const value_t<T>& get_value (const size_t i) const {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable_t::get_value] out of bounds");
            }

            // in case it is a correct index, return the value at that position
            return _table[i]._value;
        }

        // the random access operator instead returns the raw value of a value
        // type directly
        const T operator[] (const size_t i) const {
            return get_value (i).get_value ();
        }

        // return whether two tables of values are identical or not
        bool operator==(const valtable_t<T>& right) const {

            // if both tables have different sizes then they are different
            if (_table.size () != right.size ()) {
                return false;
            }

            // verify now each entry independently
            for (auto i = 0 ; i < _table.size () ; i++) {
                if (_table[i]._nbmutexes != right.get_nbmutexes (i) ||
                    _table[i]._status != right.get_status (i) ||
                    _table[i]._value != right.get_value (i)) {
                    return false;
                }
            }

            // at this point both tables have been verified to be identical
            return true;
        }

        // likewise, determine whether two tables of values are the same or not
        bool operator!=(const valtable_t<T>& right) const {

            return !((*this) == right);
        }

        // modifiers

        // insert a new value into this table. New values are enabled by default
        // and have no active mutex
        valtable_t& operator+= (const value_t<T>& value) {
            _table.push_back (_entry_t<T> {value, true, 0});
            return *this;
        }

        // insert a new value into this table, and return the index associated
        // to it. Note that its unique index corresponds to the location it
        // takes into the table. New values are enabled by default and have no
        // active mutex
        size_t insert (const value_t<T>& value) {
            _table.push_back (_entry_t<T> {value, true, 0});
            return _table.size() - 1;
        }

        // decrement the number of active mutexes of the i-th value by the given
        // delta. It returns the current number of plausible mutexes of this
        // entry after the update
        const size_t decrement_nbmutexes (const size_t i, const size_t delta=1) {

            // first, make sure the index requested is within the size of this
            // table, and that the value to decrement does not exceed the
            // current value
            if (i >= _table.size () || delta > _table[i]._nbmutexes) {
                throw std::out_of_range ("[valtable::decrement_nbmutexes] out of bounds");
            }

            // in case this is a correct operation, decrement the number of
            // active mutexes by the given amount
            _table[i]._nbmutexes -= delta;
            return _table[i]._nbmutexes;
        }

        // increment the number of plausible mutexes of the i-th value by the
        // given delta. It returns the current number of plausible mutexes of
        // this entry after the update
        const size_t increment_nbmutexes (const size_t i, const size_t delta=1) {

            // first, make sure the index requested is within the size of this
            // table
            if (i >= _table.size ()) {
                throw std::out_of_range ("[valtable::increment_nbmutexes] out of bounds");
            }

            // in case this is a correct operation, increment the number of
            // active mutexes by the given amount
            _table[i]._nbmutexes += delta;
            return _table[i]._nbmutexes;
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
