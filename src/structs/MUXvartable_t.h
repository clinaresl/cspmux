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

#include<stdexcept>
#include<vector>

#include "MUXvariable_t.h"

using namespace std;

// Class definition
//
// Definition of a table of CSP variables
class vartable_t {

    private:

        struct _entry_t {

            // INVARIANT: each entry of the table of variables stores the
            // variable, the first and last index to the values of its domain,
            // the number of plausible values in its domain (not deleted yet),
            // and also the index to the value in its domain assigned to it
            variable_t _variable;
            size_t _first, _last;
            size_t _nbvalues;
            long int _value;

            // Default constructors of entries are strictly forbidden
            _entry_t () = delete;

            // Explicit constructor - entries are built providing the variable,
            // and the first and last indices to the table of values of its
            // domain. Note that the value is initialized to an impossible
            // location in the table of values, -1, and that the number of
            // plausible values is initialized to the number of values in its
            // domain.
            _entry_t (const variable_t& variable,
                      const size_t first, const size_t last) :
                _variable { variable },
                _first { first },
                _last { last },
                _nbvalues { 1 + last - first },
                _value { -1 }
            {}

            // accessors
            const variable_t& get_variable () const {
                return _variable;
            }
            const size_t get_first () const {
                return _first;
            }
            const size_t get_last () const {
                return _last;
            }
            const size_t get_nbvalues () const {
                return _nbvalues;
            }
            const long int get_value () const {
                return _value;
            }

            // decrement the number of plausible values
            _entry_t& operator-=(const int incr) {
                _nbvalues -= incr;
                return *this;
            }

            // increment the number of plausible values
            _entry_t& operator+=(const int incr) {
                _nbvalues += incr;
                return *this;
            }

            // set the value assigned to this CSP variable
            _entry_t& operator=(const long int value) {
                _value = value;
                return *this;
            }
        };

        // INVARIANT: A table of variables consists then of a vector of entries.
        // Note that each entry is located at the index of a variable, so that
        // if the identifier of a variable is known, it can be retrieved in O
        // (1)
        vector<_entry_t> _table;

    public:

        // Tables of CSP variables are built using the default constructor
        vartable_t () :
            _table { vector<_entry_t>() }
        {}

        // Tables of CSP variables can not be copied
        vartable_t (const vartable_t& right) = delete;

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
        long int get_value (const size_t i) const;

        // modifiers

        // add a new entry to the table of variables and return its index. The
        // only necessary information is the variable itself and the first and
        // last indices to the values of its domain
        size_t add_entry (const variable_t& variable,
                          const size_t first, const size_t last) {
            _table.push_back (_entry_t (variable, first, last));
            return _table.size () - 1;
        }

        // assign the index of one value in the domain of the i-th variable to
        // it
        void assign (const size_t i, const long int value);

        // decrement the number of plausible values of the i-th variable. It
        // returns the new number of plausible values of this entry
        const size_t decrement_nbvalues (const size_t i);

        // increment the number of plausible values of the i-th variable. It
        // returns the new number of plausible values of this entry
        const size_t increment_nbvalues (const size_t i);

        // set the number of plausible values of the i-th variable to the
        // specified value. It returns the new number of plausible values of
        // this entry
        const size_t set_nbvalues (const size_t i, const size_t value);

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
