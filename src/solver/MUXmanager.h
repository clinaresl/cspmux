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

#include<string>
#include<vector>

#include "../structs/MUXmultibmap_t.h"
#include "../structs/MUXvaltable_t.h"
#include "../structs/MUXvalue_t.h"
#include "../structs/MUXvariable_t.h"
#include "../structs/MUXvartable_t.h"

using namespace std;

// Class deffinition
//
// Base definition of a manager. Note that the manager is a template because in
// can act on values defined over any type T
template<class T>
class manager {

    // INVARIANT: a manager keeps up-to-date information about variables and
    // values used in the definition of the CSP task

    // The table of values indexes all values over all variables
    valtable_t<T> _valtable;

    // The table of variables keeps up-to-date information about variables:
    // variables are indexed sequentially and for each one: the first and last
    // index to values defined in their domain are stored, in addition to the
    // remaining number of plausible values and also the index to the value
    // assigned to each variable
    vartable_t _vartable;

    // Information about mutexes and whether some values are enabled or not is
    // stored in a multibitmap. Because multibitmaps can not be created by
    // default, they are stored as a pointer
    multibmap_t* _multibmap;

    public:

        // Managers can be created only by default
        manager () :
            _valtable { valtable_t<T> () },
            _vartable { vartable_t () },
            _multibmap { nullptr }
        {}

        // Destructor
        virtual ~manager () {

            // in case a multibitmap has been created, make sure to deallocate
            // the memory it was using
            if (_multibmap) {
                delete _multibmap;
            }
        }

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
        const multibmap_t* get_multibmap () const {
            return _multibmap;
        }

        // Modifiers

        // add_variable posts a new variable and its domain to the CSP manager.
        // The domain has to be given as a vector of values. Importantly,
        // variables can not be added once constraints have been posted
        void add_variable (const variable_t& variable,
                           vector<value_t<T>> domain) {

            // Before moving further, it is mandatory to verify that the
            // information on mutexes has not been created yet ---in other
            // words, to ensure that no add_constraint has been executed. If so,
            // it is forbidden to create new variables
            if (_multibmap) {
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
            for (auto value : domain) {

                // insert this item into the table of value, and record its
                // index
                last = _valtable.insert(value);

                // if this is the first insertion, then update the value of the
                // first index
                first = (first == std::string::npos) ? last : first;
            }

            // next, add this variable to the table of CSP variables along with
            // the bounds of its domain
            _vartable.add_entry (variable, first, last);

        }

        // add_constraint invokes the function given in first place over all
        // values of the domains of the given variables. Every combination of
        // values which makes the function to return false is stored as a mutex
        void add_constraint (typename value_t<T>::constraintHandler* func,
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

            // Next, in case the multibitmap storing all mutexes has not been
            // created yet, do it now
            if (!_multibmap) {

                // the length and width of the multibitmap has to be strictly
                // equal to the overall number of values registered in this
                // manager
                _multibmap = new multibmap_t (_valtable.size (), _valtable.size ());
            }

            // Now comes the fun: for all combination of values (a, b) in the
            // domains of each CSP variable, a in var1, b in var2, invoke the
            // constraint
            for (int i = _vartable.get_first(index1) ;
                 i <= _vartable.get_last(index1) ;
                 i++) {

                for (int j = _vartable.get_first(index2) ;
                     j <= _vartable.get_last(index2) ;
                     j++) {

                    // if the constraint returns false, then a mutex has been
                    // found
                    if (!(*func) (_valtable.get(i).get_value (),
                                  _valtable.get(j).get_value ())) {

                        // set this mutex in the multibitmap
                        _multibmap->set (i, j, true);
                    }
                }
            }
        }
};

#endif // _MUXMANAGER_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
