// -*- coding: utf-8 -*-
// TSTmanager.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 11-08-2021 12:59:25.899586037 (1628679565)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of managers

#include "../fixtures/TSTmanagerfixture.h"

// Checks the creation of a manager creates empty tables and bitmap
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, EmptyManager) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // and verify it contains empty tables of variables and values
        ASSERT_EQ (m.get_vartable ().size (), 0);
        ASSERT_EQ (m.get_valtable ().size (), 0);

        // finally, make sure also that the multibitmap is empty
        ASSERT_EQ (m.get_multibmap(), nullptr);
    }
}

// Checks that adding variables to a manager produces the expected results
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, AddVariablesManager) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarVals (NB_VARIABLES, names, values);

        // now, insert all these variables
        for (int i = 0 ; i < NB_VARIABLES ; i++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[i]), values[i]);
        }

        // verify now that all variables have been properly initialized. First,
        // check the table of values
        valtable_t<int> valtable = m.get_valtable ();
        for (int i = 0, idx = 0 ; i < NB_VARIABLES ; i++ ) {
            for (int j = 0 ; j < values[i].size () ; j++, idx++) {

                // retrieve the value at the location pointed to by the current
                // index
                value_t<int> value = valtable.get (idx);

                // and verify this is the expected value
                ASSERT_EQ (value.get_value(), values[i][j].get_value ());
            }
        }

        // now, verify the contents of the table of variables are also ccorrect
        vartable_t vartable = m.get_vartable ();
        for (int i = 0, idx = 0 ; i < NB_VARIABLES ; idx += values[i++].size ()) {

            // verify the name
            ASSERT_EQ (vartable.get_variable (i).get_name (), names[i]);

            // verify also that the bounds of the domain have been properly
            // recorded
            ASSERT_EQ (vartable.get_first (i), idx);
            ASSERT_EQ (vartable.get_last (i), idx + values[i].size ()-1);

            // check that the number of plausible values is also correctly
            // stored
            ASSERT_EQ (vartable.get_nbvalues (i), values[i].size ());

            // finally, verify that no variable get a value assigned
            ASSERT_EQ (vartable.get_value (i), -1);
        }

        // Before leaving this case, ensure that the multibitmap is still null
        ASSERT_EQ (m.get_multibmap(), nullptr);
    }
}

vector<pair<int, int>> parameters;

// Checks that constraints are invoked using the right order of values. In
// passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int i = 0 ; i < NB_VARIABLES/10 ; i++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[i]), values[i]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        parameters.clear ();
        m.add_constraint([] (int val1, int val2) {
            parameters.push_back (pair<int, int>{val1, val2});
            return true;
        },variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multibitmap is not null anymore
        ASSERT_NE (m.get_multibmap(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        ASSERT_THROW (m.add_variable (variable_t ("not allowed!"),
                                      values[0]),
                      runtime_error);

        // finally, verify that this is the expected cross-product
        ASSERT_EQ (parameters, cross_product (variables[0], variables[1], values));
    }
}

// Checks that posting constraints on unregistered variables raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, UnregisteredVariableMutexIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int i = 0 ; i < NB_VARIABLES/10 ; i++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[i]), values[i]);
        }

        // create two variables which are guaranteed (randomly though) to not
        // exist
        variable_t variable0 = variable_t ("This variable is not expected to exist!");
        variable_t variable1 = variable_t ("This variable is not expected to exist as well!");

        // Add now a dummy constraint (in the form of a lambda function) but
        // invoke it over non-existing variables and verify the operation raises
        // an exception
        ASSERT_THROW(m.add_constraint([] (int val1, int val2) {
            return true;
        }, variable0, variable1),
            invalid_argument);
    }
}

int quotient;

// Checks that constraints properly acknowledge mutexes and store them
// accordingly
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, MutexIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int i = 0 ; i < NB_VARIABLES/10 ; i++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[i]), values[i]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex
        quotient = 1 + rand ()%6;
        m.add_constraint([] (int val1, int val2) {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized

        // access the manager multibitmap. It is guarantted to be non-null.
        // Indeed one unit test before this one already verified this
        const multibmap_t* multibmap = m.get_multibmap();

        // get the index to the first and last value in the domain of each
        // variable
        pair<int, int> bounds1 = get_bounds (variables[0], values);
        pair<int, int> bounds2 = get_bounds (variables[1], values);

        // to double check that the boundaries have been properly computed,
        // assert their equality
        ASSERT_EQ (m.get_vartable().get_first(variables[0]),
                   bounds1.first);
        ASSERT_EQ (m.get_vartable().get_last(variables[0]),
                   bounds1.second);
        ASSERT_EQ (m.get_vartable().get_first(variables[1]),
                   bounds2.first);
        ASSERT_EQ (m.get_vartable().get_last(variables[1]),
                   bounds2.second);

        // and now verify the values of all mutexies
        for (int idx1 = bounds1.first ; idx1 < bounds1.second ; idx1++) {
            for (int idx2 = bounds2.first ; idx2 < bounds2.second ; idx2++) {

                // get the values stored in the multibitmap
                size_t num1 = values[variables[0]][idx1-bounds1.first].get_value ();
                size_t num2 = values[variables[1]][idx2-bounds2.first].get_value ();

                // and verify that only those cases where the sum is divisible
                // by 7 have been recognized as mutexes. Note the case
                // quotient=6 is also acknowledged as in this case, no pair is
                // mutex
                if (quotient==6) {
                    ASSERT_FALSE (multibmap->get (idx1, idx2));
                } else {
                    ASSERT_EQ (multibmap->get (idx1, idx2),
                               (num1 + num2) % quotient == 0);
                }
            }
        }
    }
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
