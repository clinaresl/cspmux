// -*- coding: utf-8 -*-
// MUXvartable_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 16:02:39.548401922 (1628517759)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// A table of CSP variables record the indexes of every variable along with
// information about them

#ifndef _MUXVARTABLE_T_H_
#define _MUXVARTABLE_T_H_

#include<map>
#include<stdexcept>
#include<string>
#include<vector>

#include "MUXvariable_t.h"

// using namespace std;

// Class definition
//
// Definition of a table of CSP variables
class vartable_t {

    private:

        struct _entry_t {

            // INVARIANT: each entry of the table of variables stores the
            // variable, the first and last index to the values of its domain,
            // the number of plausible values in its domain (not deleted yet),
            // and also the index to the value in its domain assigned to it,
            // which takes the maximum value by default
            variable_t _variable;
            size_t _first, _last;
            size_t _nbvalues;
            size_t _value;

            // Default constructors of entries are strictly forbidden
            _entry_t () = delete;

            // default copy and move constructors
            _entry_t (_entry_t&) = default;
            _entry_t (_entry_t&&) = default;

            // default copy and move assignments
            _entry_t& operator=(_entry_t&) = default;
            _entry_t& operator=(_entry_t&&) = default;

            // Explicit constructor - entries are built providing the variable,
            // and the first and last indices to the table of values of its
            // domain. Note that the value is initialized to an impossible
            // location in the table of values, the maximum size_t, and that the
            // number of plausible values is initialized to the number of values
            // in its domain.
            _entry_t (variable_t& variable,
                      const size_t first, const size_t last) :
                _variable { variable },
                _first { first },
                _last { last },
                _nbvalues { 1 + last - first },
                _value { std::string::npos }
            {}

            // set the value assigned to this CSP variable
            _entry_t& operator=(const size_t value) {
                _value = value;
                return *this;
            }

            // return whether two entries are the same or not
            bool operator==(const _entry_t& right) const {
                return _variable == right._variable &&
                    _first == right._first &&
                    _last == right._last &&
                    _nbvalues == right._nbvalues &&
                    _value == right._value;
            }

            // Likewise, define whether they are different
            bool operator!=(const _entry_t& right) const {
                return _variable != right._variable ||
                    _first != right._first ||
                    _last != right._last ||
                    _nbvalues != right._nbvalues ||
                    _value != right._value;
            }

        };

        // INVARIANT: A table of variables consists then of a vector of entries.
        // Note that each entry is located at the index of a variable, so that
        // if the identifier of a variable is known, it can be retrieved in O(1)
        std::vector<_entry_t> _table;

        // variables are identified by their name which has to be unique. The
        // following map keeps information about all variables registered in
        // this table. It associates each name with a location in the table
        // where they are registered
        std::map<std::string, size_t> _indices;

    public:

        // Tables of CSP variables are built using the default constructor
        vartable_t () :
            _table { std::vector<_entry_t>() },
            _indices { std::map<std::string, size_t>() }
        {}

        // accessors

        // return the variable in the i-th location of this table
        const variable_t& get_variable (const size_t i) const;

        // return the index to the first value in the domain of the i-th
        // variable
        const size_t get_first (const size_t i) const;

        // return the index to the last value in the domain of the i-th
        // variable
        const size_t get_last (const size_t i) const;

        // return the number of plausible values in the domain of the i-th
        // variable
        const size_t get_nbvalues (const size_t i) const;

        // return the index to the value assigned to the i-th variable. If none
        // has been assigned yet then -1 is returned
        const size_t get_value (const size_t i) const;

        // return whether two tables of CSP variables are identical or not
        bool operator==(const vartable_t& right) const;

        // likewise, define whether they are different
        bool operator!=(const vartable_t& right) const;

        // variables are indexed by their name. The following operator returns
        // the index of any variable registered in the table. In case
        // it does not exist, an exception is thrown
        const size_t operator[] (const std::string& name) const;

        // likewise, it is also possible to access the name of a variable by its
        // location through the same operator. In case it does not exist, an
        // exception is thrown
        const variable_t& operator[] (const size_t i) const {
            if (i >= _table.size ()) {
                throw std::out_of_range ("[const variable_t& vartable_t::operator[size_t]] out of bounds");
            }
            return _table[i]._variable;
        }

        // modifiers

        // add a new entry to the table of variables and return its index. The
        // only necessary information is the variable itself and the first and
        // last indices to the values of its domain. In case
        //
        // In case the variable already exists or the domain is empty an
        // exception israised
        size_t insert (variable_t& variable,
                       const size_t first, const size_t last);

        // assign the index of one value in the domain of the a variable to it
        void assign (const size_t variable, const size_t value);

        // decrement the number of plausible values of the i-th variable. It
        // returns the new number of plausible values of this entry
        //
        // INVARIANT: the number of plausible values is guaranteed to be
        // non-negative
        const size_t decrement_nbvalues (const size_t i, const size_t delta = 1);

        // increment the number of plausible values of the i-th variable. It
        // returns the new number of plausible values of this entry
        const size_t increment_nbvalues (const size_t i, const size_t delta = 1);

        // set the number of plausible values of the i-th variable to the
        // specified value. It returns the new number of plausible values of
        // this entry
        const size_t set_nbvalues (const size_t i, const size_t nbvalues);

        // capacity

        // return the number of entries in this table
        size_t size () const {
            return _table.size ();
        }
};

#endif // _MUXVARTABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
