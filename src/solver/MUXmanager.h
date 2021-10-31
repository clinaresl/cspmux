// -*- coding: utf-8 -*-
// MUXmanager.h
// -----------------------------------------------------------------------------
//
// Started on <mar 10-08-2021 20:38:50.398884100 (1628620730)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Base definition used for defining algorithms to solve CSP problems based on
// binary mutexes

#ifndef _MUXMANAGER_H_
#define _MUXMANAGER_H_

#include<memory>
#include<set>
#include<string>
#include<vector>

#include "../structs/MUXmultivector_t.h"
#include "../structs/MUXvaltable_t.h"
#include "../structs/MUXvalue_t.h"
#include "../structs/MUXvariable_t.h"
#include "../structs/MUXvartable_t.h"
#include "../solver/MUXsstack_t.h"

using namespace std;

// Class deffinition
//
// Base definition of a manager. Note that the manager is a template because in
// can act on values defined over any type T
template<class T>
class manager {

    private:

        // INVARIANT: a manager keeps up-to-date information about variables and
        // values used in the definition of the CSP task

        // The table of values indexes all values over all variables and stores also
        // the number of still applicable mutexes (i.e., values which have not been
        // disabled yet)
        valtable_t<T> _valtable;

        // The table of variables keeps up-to-date information about variables:
        // variables are indexed sequentially and for each one: the first and last
        // index to values defined in their domain are stored, in addition to the
        // remaining number of plausible values and also the index to the value
        // assigned to each variable
        vartable_t _vartable;

        // Information about mutexes and whether some values are enabled or not is
        // stored in a multivector. Because multivectors can not be created by
        // default, they are stored as a pointer
        unique_ptr<multivector_t> _multivector;

        // the following private function performs a binary search over the
        // table of variables to determine the variable a specific value belongs
        // to. 'value' is the index of the value to look for; lower and upper
        // are the bounds of the scope of the binary search over the table of
        // variables thus they are indices to variables
        size_t _val_to_var (const size_t value, const size_t lower, const size_t upper) const {

            // take the midpoint between the lower and upper bounds. 'mid' is
            // the index to a variable.
            size_t mid = (lower + upper) / 2;

            // first, if the value we are currently looking for falls strictly
            // below the mid point
            if (value < _vartable.get_first(mid)) {

                // then proceed recursively with the lower half excluding the
                // mid point
                return _val_to_var (value, lower, mid-1);
            }

            // if it is strictly above the mid point
            if (value > _vartable.get_last(mid)) {

                // then proceed recursively with the upper half excluding the
                // mid point
                return _val_to_var (value, mid+1, upper);
            }

            // the last case is that the value we are currently looking for is
            // right in the mid point, so return it
            return mid;
        }

    public:

        // Managers can be created only by default
        manager () :
            _valtable { valtable_t<T> () },
            _vartable { vartable_t () },
            _multivector { nullptr }
        {}

        // Accessors

        // the following service is provided for testing purposes
        const valtable_t<T>& get_valtable () const {
            return _valtable;
        }

        // the following service is provided for testing purposes
        const vartable_t& get_vartable () const {
            return _vartable;
        }

        // the following service is provided for testing purposes
        const unique_ptr<multivector_t>& get_multivector () const {
            return _multivector;
        }

        // return the variable a specific value belongs to. If the given index
        // exceeds the current number of values an exception is thrown
        size_t val_to_var (const size_t value) const {

            // first things first. If this index exceeds the table of values,
            // immediately raise an exception
            if (value >= _valtable.size ()) {
                throw out_of_range ("[manager::val_to_var] Out of bounds");
            }

            // otherwise, simply perform a binary search over the table of
            // variables
            return _val_to_var (value, 0, _vartable.size ()-1);
        }

        // Modifiers

        // add_variable posts a new variable and its domain to the CSP manager.
        // The domain has to be given as a vector of values. Importantly,
        // variables can not be added once constraints have been posted. Also,
        // no value can be repeated in the domain of a variable.
        void add_variable (variable_t& variable,
                           vector<value_t<T>>& domain) {

            // Before moving further, it is mandatory to verify that the
            // information on mutexes has not been created yet ---in other
            // words, to ensure that no add_constraint has been executed. If so,
            // it is forbidden to create new variables
            if (_multivector) {
                throw runtime_error ("[manager::add_variable] It is forbidden to add variables after adding constraints!");
            }

            // Adding a variable is a simple process: first, all values are
            // registered in the table of values. Indices to the first and last
            // element are recorded, and they are given when registering the
            // given variable in the table of variables

            // store all values in the table of values of this manager.
            // Initialize the first location to npos, the largest value so that
            // it serves as a token meaning "unitialized"
            size_t first = std::string::npos;
            size_t last;
            set<value_t<T>> values;
            for (auto value : domain) {

                // to ensure that no value in the domain is repeated, a set is
                // used to remember those values already inserted and to look it
                // up prior to new insertions
                if (values.find (value) != values.end ()) {
                    throw runtime_error ("[manager::addvariable_t] Repeated value in the domain of a variable");
                }

                // insert this item into the table of value, and record its
                // index. Remember it also to avoid duplicates
                last = _valtable.insert(value);
                values.insert (value);

                // if this is the first insertion, then update the value of the
                // first index
                first = (first == std::string::npos) ? last : first;
            }

            // next, add this variable to the table of CSP variables along with
            // the bounds of its domain
            _vartable.insert (variable, first, last);
        }

        // add_constraint invokes the function given in first place over all
        // values of the domains of the given variables. Every combination of
        // values which makes the function to return false is stored as a mutex.
        //
        // Mutexes are represented with functions that might be either
        // commutative or not. In case they are commutative, then a mutex could
        // be defined over the same variable, i.e., func (Xi, Xi). However, this
        // does not make any sense. Thus, in case a mutex is invoked over the
        // same variables, then an exception is immediately raised
        //
        // Likewise, this solver assumes mutexes to be reflexive. Thus, it is
        // strictly forbidden to invoke the same constraint over the same set of
        // variables with different orderings, i.e., add_constraint (func, Xi,
        // Xj) and add_constraint (func, Xj, Xi) are strictly equivalent and
        // invoking both might cause unpredictable effects ---the reason being
        // that the same mutexes are stored more than once, but the search
        // algorithm should remove them only once
        template<typename Handler>
        void add_constraint (Handler func,
                             const variable_t& var1, const variable_t& var2) {


            // Before moving further, verify the given variables exist
            size_t index1, index2;
            try {
                index1 = _vartable[var1.get_name ()];
                index2 = _vartable[var2.get_name ()];
            } catch (runtime_error e) {

                // if operator[] raised an exception, then this variable has not
                // been registered
                throw invalid_argument ("[manager::add_constraint] Unregistered variable");
            }

            // verify that both variables are different
            if (index1 == index2) {
                throw invalid_argument {"[manager::add_constraint] Constraints can not be defined over the same variable"};
            }

            // Next, in case the multivector storing all mutexes has not been
            // created yet, do it now
            if (!_multivector) {

                // the length of the multivector has to be strictly equal to the
                // overall number of values registered in this manager
                _multivector = unique_ptr<multivector_t>{new multivector_t (_valtable.size ())};
            }

            // Now comes the fun: for all combination of values (a, b) in the
            // domains of each CSP variable, a in var1, b in var2, invoke the
            // constraint
            for (size_t i = _vartable.get_first(index1) ;
                 i <= _vartable.get_last(index1) ;
                 i++) {

                for (size_t j = _vartable.get_first(index2) ;
                     j <= _vartable.get_last(index2) ;
                     j++) {

                    // if the constraint returns false, then a mutex has been
                    // found
                    if (!(func) (_valtable[i], _valtable[j])) {

                        // set this mutex in the multibitmap. Note this solver
                        // only allows mutexes which are reflexive
                        _multivector->set (i, j);
                        _multivector->set (j, i);

                        // and update the number of mutexes of these entries
                        //
                        // WARNING! adding constraints again over the same set
                        // of variables previously used but with different
                        // orderings would cause unpredictable effects!
                        _valtable.increment_nbmutexes (i);
                        _valtable.increment_nbmutexes (j);
                    }
                }
            }
        }

        // Handlers

        // The following handler restores the number of feasible values of one
        // variable to the previous one. It receives the index to the variable
        // to update, the previous number of feasible values and the new one. In
        // case its current number of feasible values is not the last one, then
        // an exception is immediately raised
        void set_var_nbvalues (size_t i, size_t prev, size_t last) {

            // check the current number of feasible values of this variable is
            // the one assigned last
            if (_vartable.get_nbvalues(i) != last) {
                throw runtime_error ("[manager::set_var_nbvalues] Inconsistency found!");
            }

            // and now update it
            _vartable.set_nbvalues (i, prev);
        }

        // The following handler restores the value assigned to a variable to
        // the previous one. It receives the index to the variable to update,
        // its previous value and the new one. In case its current value is not
        // the new one, then an exception is immediately raised
        void set_var_value (size_t i, size_t prev, size_t last) {

            // check the current value assigned to this variable is the one
            // assigned last
            if (_vartable.get_value(i) != last) {
                throw runtime_error ("[manager::set_var_value] Inconsistency found!");
            }

            // and now update it
            _vartable.assign (i, prev);
        }

        // The following handler restores the status of one value to the
        // previous one. It receives the index to the value to update, the
        // previous status and the last one. In case its current status is not
        // the last one, then an exception is immediately raised
        void set_val_status (size_t i, size_t prev, size_t last) {

            // check the current status of the specified value is the one
            // assigned last
            if (_valtable.get_status (i) != last) {
                throw runtime_error ("[manager::set_val_status] Inconsistency found!");
            }

            // and now update it
            _valtable.set_status (i, prev);
        }

        // The following handler restores the number of feasible mutexes of a
        // specific value to the previous one. It receives the index to the
        // value to update, the previous number of feasible mutexes, and the
        // last one. In case its current number of feasible values is not the
        // last one, then an exception is immediately raised
        void set_val_nbmutexes (size_t i, size_t prev, size_t last) {

            // check the current number of feasible values is the one assigned
            // last
            if (_valtable.get_nbmutexes (i) != last) {
                throw runtime_error ("[manager::set_val_nbvalues] Inconsistency found!");
            }

            // and now update it
            _valtable.set_nbmutexes (i, prev);
        }
};

#endif // _MUXMANAGER_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
