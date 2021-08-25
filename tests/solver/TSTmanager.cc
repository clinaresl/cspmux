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

#include<functional>

#include "../fixtures/TSTmanagerfixture.h"

// Checks the creation of a manager creates empty tables and multivector
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, EmptyManager) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // and verify it contains empty tables of variables and values
        ASSERT_EQ (m.get_vartable ().size (), 0);
        ASSERT_EQ (m.get_valtable ().size (), 0);

        // finally, make sure also that the multivector is empty
        ASSERT_EQ (m.get_multivector(), nullptr);
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
        randVarIntVals (NB_VARIABLES, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // verify now that all variables have been properly initialized. First,
        // check the table of values
        valtable_t<int> valtable = m.get_valtable ();
        for (int j = 0, idx = 0 ; j < NB_VARIABLES ; j++ ) {
            for (int k = 0 ; k < values[j].size () ; k++, idx++) {

                // retrieve the value at the location pointed to by the current
                // index
                value_t<int> value = valtable.get_value (idx);

                // and verify this is the expected value
                ASSERT_EQ (value.get_value(), values[j][k].get_value ());
            }
        }

        // now, verify the contents of the table of variables are also ccorrect
        vartable_t vartable = m.get_vartable ();
        for (int j = 0, idx = 0 ; j < NB_VARIABLES ; idx += values[j++].size ()) {

            // verify the name
            ASSERT_EQ (vartable.get_variable (j).get_name (), names[j]);

            // verify also that the bounds of the domain have been properly
            // recorded
            ASSERT_EQ (vartable.get_first (j), idx);
            ASSERT_EQ (vartable.get_last (j), idx + values[j].size ()-1);

            // check that the number of plausible values is also correctly
            // stored
            ASSERT_EQ (vartable.get_nbvalues (j), values[j].size ());

            // finally, verify that no variable get a value assigned
            ASSERT_EQ (vartable.get_value (j), -1);
        }

        // Before leaving this case, ensure that the multivector is still null
        ASSERT_EQ (m.get_multivector(), nullptr);
    }
}

// Verify the bounds on the domains of all variables in the table of variables
// are contiguous
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ContiguousDomainsManager) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarIntVals (NB_VARIABLES, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[i]);
        }

        // check the domains of all variables: check the first value of the
        // first value is strictly equal to 0; next, verify that the last index
        // of the domain of the j-th variable is right before the first index of
        // the domain of the next variable; finally, verify that the last value
        // of the last variable is indeed the index to the last value in the
        // table of values
        ASSERT_EQ (m.get_vartable ().get_first (0), 0);
        for (auto j = 0 ; j < NB_VARIABLES-1 ; j++ ) {

            // verify that the domains are contiguous
            ASSERT_EQ (1 + m.get_vartable ().get_last (j),
                       m.get_vartable ().get_first (j+1));
        }
        ASSERT_EQ (m.get_vartable ().get_last (NB_VARIABLES-1),
                   m.get_valtable ().size ()-1);
    }
}

// // Checks that posting constraints on unregistered variables raises an exception
// // ----------------------------------------------------------------------------
TEST_F (ManagerFixture, UnregisteredVariablesManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarIntVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // create two variables, one at least being unregistered. For this,
        // throw a dice and decide among different cases
        variable_t variable0("This variable is not expected to exist!");
        variable_t variable1("This variable is not expected to exist as well!");
        int basecase = rand () % 3;
        switch (basecase) {
            case 1:
                variable1 = variable_t (names[rand () % NB_VARIABLES/10]);
                break;
            case 2:
                variable0 = variable_t (names[rand () % NB_VARIABLES/10]);
                break;
        }

        // Add now a dummy constraint (in the form of a lambda function) but
        // invoke it over at least one non-existing variable and verify the
        // operation raises an exception
        ASSERT_THROW(m.add_constraint([] (int val1, int val2) {
            return true;
        }, variable0, variable1),
            invalid_argument);
    }
}

int quotient;

// Checks that conversion from values to variables is correct
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ValToVarManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarIntVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex. Post
        // this constraint over the first two variables
        quotient = 1 + rand ()%6;
        m.add_constraint([] (int val1, int val2) {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[0]}, variable_t{names[1]});

        // Whether the mutexes have been properly registered or not is the goal
        // of another unit test. In this case, we are simply going to test that
        // conversion from values to variables is correct. Randomly chose a
        // variable and a value within its domain
        size_t variable = rand () % names.size ();
        size_t value = m.get_vartable ().get_first (variable) +
            rand () % (1+m.get_vartable().get_last(variable) -
                       m.get_vartable ().get_first (variable));

        // now, check the variable this value belongs to is correct
        ASSERT_EQ (m.val_to_var(value), variable);
        ASSERT_TRUE (m.get_vartable().get_first(variable) <= value);
        ASSERT_TRUE (m.get_vartable().get_last(variable) >= value);

        // in passing, check also the bound cases
        ASSERT_EQ (m.val_to_var (m.get_vartable ().get_first (variable)), variable);
        ASSERT_EQ (m.val_to_var (m.get_vartable ().get_last (variable)), variable);

    }
}

// Checks that constraints properly acknowledge mutexes and store them
// accordingly in managers of integers
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ValNbvaluesManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarIntVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // verify the number of values is not null
        ASSERT_TRUE (m.get_valtable ().size () > 0);

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // before moving further, make sure that the number of enabled mutexes
        // of all values is exactly 0
        for (int j = 0 ; j < m.get_valtable ().size () ; j++) {
            ASSERT_EQ (m.get_valtable ().get_nbvalues (j), 0);
        }

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex.
        // Constraints are defined over two variables randomly selected
        quotient = 1 + rand ()%6;
        m.add_constraint([] (int val1, int val2) {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // once constraints have been added, new mutexes have been discovered
        // (in this case, admittedly, only over two different variables and not
        // all, so that in many cases we'll have 0 mutexes but for the values of
        // the selected variables we'll have an arbitrary number of them). Make
        // sure the number of enabled mutexes is strictly equal to the number of
        // mutexes stored in each value
        multivector_t* multivector = m.get_multivector ();
        for (size_t j = 0 ; j < m.get_valtable ().size () ; j++) {
            ASSERT_EQ (multivector->get (j).size (), m.get_valtable ().get_nbvalues (j));
        }
    }
}


vector<pair<int, int>> parametersInt;

// Checks that constraints are invoked using the right order of int values. In
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
        randVarIntVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        parametersInt.clear ();
        m.add_constraint([] (int val1, int val2) {
            parametersInt.push_back (pair<int, int>{val1, val2});
            return true;
        },variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        ASSERT_THROW (m.add_variable (variable_t ("not allowed!"),
                                      values[0]),
                      runtime_error);

        // finally, verify that this is the expected cross-product
        ASSERT_EQ (parametersInt, cross_product<int> (variables[0], variables[1], values));
    }
}

// Checks that constraints properly acknowledge mutexes and store them
// accordingly in managers of integers
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, MutexIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<int>>> values;
        randVarIntVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
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
        multivector_t* multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<int, int> bounds1 = get_bounds<int> (variables[0], values);
        pair<int, int> bounds2 = get_bounds<int> (variables[1], values);

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
                    ASSERT_FALSE (multivector->find (idx1, idx2));
                } else {

                    // multivectors store mutexes explicitly and thus, if idx2
                    // is not a mutex of idx1 it is just not present in its
                    // vector of mutexes
                    ASSERT_TRUE ( ( multivector->find (idx1, idx2) && (num1 + num2) % quotient == 0 ) ||
                                  (!multivector->find (idx1, idx2) && (num1 + num2) % quotient != 0));
                }
            }
        }
    }
}

vector<pair<string, string>> parametersString;

// Checks that constraints are invoked using the right order of string values.
// In passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintStringManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<string> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<string>>> values;
        randVarStringVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        parametersString.clear ();
        m.add_constraint([] (string val1, string val2) {
            parametersString.push_back (pair<string, string>{val1, val2});
            return true;
        },variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        ASSERT_THROW (m.add_variable (variable_t ("not allowed!"),
                                      values[0]),
                      runtime_error);

        // finally, verify that this is the expected cross-product
        ASSERT_EQ (parametersString, cross_product<string> (variables[0], variables[1], values));
    }
}

// Checks that constraints properly acknowledge mutexes and store them
// accordingly in managers of strings
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, MutexStringManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<string> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<string>>> values;
        randVarStringVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if any of the strings contains the letter 'r'
        m.add_constraint([] (string val1, string val2) {
            return !val1.find ('r') && !val2.find ('r');
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized

        // access the manager multibitmap. It is guarantted to be non-null.
        // Indeed one unit test before this one already verified this
        multivector_t* multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<int, int> bounds1 = get_bounds<string> (variables[0], values);
        pair<int, int> bounds2 = get_bounds<string> (variables[1], values);

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
                string str1 = values[variables[0]][idx1-bounds1.first].get_value ();
                string str2 = values[variables[1]][idx2-bounds2.first].get_value ();

                // and verify that only those cases where either argument
                // contains the letter 'r' is recognized as a mutex. Note that
                // multivectors store mutexes explicitly
                ASSERT_TRUE ( (multivector->find (idx1, idx2) && (str1.find ('r') || str2.find ('r'))) ||
                              (!multivector->find (idx1, idx2) && !str1.find ('r') && !str2.find ('r') ));
            }
        }
    }
}

vector<pair<time_t, time_t>> parametersTime;

// Checks that constraints are invoked using the right order of time values.
// In passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintTimeManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<time_t> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        randVarTimeVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        parametersTime.clear ();
        m.add_constraint([] (time_t val1, time_t val2) {
            parametersTime.push_back (pair<time_t, time_t>{val1, val2});
            return true;
        },variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        ASSERT_THROW (m.add_variable (variable_t ("not allowed!"),
                                      values[0]),
                      runtime_error);

        // finally, verify that this is the expected cross-product
        ASSERT_EQ (parametersTime, cross_product<time_t> (variables[0], variables[1], values));
    }
}

// Checks that constraints properly acknowledge mutexes and store them
// accordingly in managers of time
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, MutexTimeManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<time_t> m;

        // randomly pick up information for the variables to insert
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        randVarTimeVals (NB_VARIABLES/10, names, values);

        // now, insert all these variables
        for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

            // add the i-th variable along with its domain
            m.add_variable (variable_t (names[j]), values[j]);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, NB_VARIABLES/10);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex
        quotient = 1 + rand ()%6;
        m.add_constraint([] (time_t val1, time_t val2) {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized

        // access the manager multibitmap. It is guarantted to be non-null.
        // Indeed one unit test before this one already verified this
        multivector_t* multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<int, int> bounds1 = get_bounds<time_t> (variables[0], values);
        pair<int, int> bounds2 = get_bounds<time_t> (variables[1], values);

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
                    ASSERT_FALSE (multivector->find (idx1, idx2));
                } else {
                    ASSERT_TRUE ( ( multivector->find (idx1, idx2) && (num1 + num2) % quotient == 0 ) ||
                                  (!multivector->find (idx1, idx2) && (num1 + num2) % quotient != 0));
                }
            }
        }
    }
}

manager<int> mVarNbValues;
void handler_var_nbvalues (size_t index, size_t val1, size_t val2) {
    mVarNbValues.set_var_nbvalues (index, val1, val2);
}

// Check that restoring the number of values in the domain of a variable works
// correctly
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, RestoreVariableNbvaluesManager) {

    // first, update the manager with variables and values

    // randomly pick up information for the variables to insert
    vector<string> names;
    vector<vector<value_t<int>>> values;
    randVarIntVals (NB_VARIABLES/10, names, values);

    // now, insert all these variables
    for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

        // add the i-th variable along with its domain
        mVarNbValues.add_variable (variable_t (names[j]), values[j]);
    }

    // now, performe the tests
    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty stack
        sstack_t stack;

        // randomly choose one variable to update and the new value
        size_t variable = rand () % (NB_VARIABLES/10);
        size_t last = rand () % NB_VALUES;

        // and make a backup copy of the number of values in the domain of the
        // selected variable
        size_t prev = mVarNbValues.get_vartable ().get_nbvalues (variable);

        // first, create an empty frame
        frame_t frame;

        // now, create an action to restore the value of the number of values in
        // the domain of the selected variable before updating it
        action_t action {handler_var_nbvalues,
            variable,
            prev,
            last};
        frame.push (action);

        // Update the number of feasible values. For this, use the handler
        // defined above but reversing the arguments
        handler_var_nbvalues (variable, last, prev);

        // verify that the table of variables has been properly updated
        ASSERT_EQ (mVarNbValues.get_vartable().get_nbvalues(variable), last);

        // push the frame onto the stack, verify it has indeed one frame, and
        // immediately after unwind it
        stack.push (frame);
        ASSERT_EQ (stack.size (), 1);
        stack.unwind ();

        // and now check the number of values in the domain of this variable has
        // been restored to the previous one, and that the stack is empty now
        ASSERT_EQ (mVarNbValues.get_vartable ().get_nbvalues (variable), prev);
        ASSERT_EQ (stack.size (), 0);
    }
}

manager<int> mValue;
void handler_var_value (size_t index, size_t val1, size_t val2) {
    mValue.set_var_value (index, val1, val2);
}

// Check that restoring the number of values in the domain of a variable works
// correctly
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, RestoreVariableValueManager) {

    // first, update the manager with variables and values

    // randomly pick up information for the variables to insert
    vector<string> names;
    vector<vector<value_t<int>>> values;
    randVarIntVals (NB_VARIABLES/10, names, values);

    // now, insert all these variables
    for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

        // add the i-th variable along with its domain
        mValue.add_variable (variable_t (names[j]), values[j]);
    }

    // now, performe the tests
    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty stack
        sstack_t stack;

        // randomly choose one variable to update and the new value
        size_t variable = rand () % (NB_VARIABLES/10);
        size_t last = rand () % NB_VALUES;

        // and make a backup copy of the current value of the selected variable
        size_t prev = mValue.get_vartable ().get_value (variable);

        // first, create an empty frame
        frame_t frame;

        // now, create an action to restore the value of the selected variable
        // before updating it
        action_t action {handler_var_value,
            variable,
            prev,
            last};
        frame.push (action);

        // Update the value of this variable. For this, use the handler defined
        // above but reversing the arguments
        handler_var_value (variable, last, prev);

        // verify that the table of variables has been properly updated
        ASSERT_EQ (mValue.get_vartable().get_value(variable), last);

        // push the frame onto the stack, verify it has indeed one frame, and
        // immediately after unwind it
        stack.push (frame);
        ASSERT_EQ (stack.size (), 1);
        stack.unwind ();

        // and now check the value assigned to this variable has been restored
        // to the previous one, and that the stack is empty now
        ASSERT_EQ (mValue.get_vartable ().get_value (variable), prev);
        ASSERT_EQ (stack.size (), 0);
    }
}

manager<int> mStatus;
void handler_val_status (size_t index, size_t val1, size_t val2) {
    mStatus.set_val_status (index, val1, val2);
}

// Check that restoring the number of values in the domain of a variable works
// correctly
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, RestoreValueStatusManager) {

    // first, update the manager with variables and values

    // randomly pick up information for the variables to insert
    vector<string> names;
    vector<vector<value_t<int>>> values;
    randVarIntVals (NB_VARIABLES/10, names, values);

    // now, insert all these variables
    for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

        // add the i-th variable along with its domain
        mStatus.add_variable (variable_t (names[j]), values[j]);
    }

    // for the table of values to be initialized it is mandatory to post
    // constraints. The following post a dummy constraint to the first two
    // variables
    mStatus.add_constraint([] (int val1, int val2) {
        return true;
    }, variable_t{names[0]}, variable_t{names[1]});
    ASSERT_NE (mStatus.get_multivector (), nullptr);

    // now, performe the tests
    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty stack
        sstack_t stack;

        // randomly choose one value to update and the new status
        size_t value = rand () % mStatus.get_multivector ()->size ();
        size_t last = 0;

        // and make a backup copy of the current status of the selected value
        size_t prev = mStatus.get_multivector ()->get_status (value);

        // in passing ensure that all values are enabled by default
        ASSERT_EQ (prev, 1);

        // first, create an empty frame
        frame_t frame;

        // now, create an action to restore the status of the selected value
        // before updating it
        action_t action {handler_val_status,
            value,
            prev,
            last};
        frame.push (action);

        // Update the status of this value. For this, use the handler defined
        // above but reversing the arguments
        handler_val_status (value, last, prev);

        // verify that the table of variables has been properly updated
        ASSERT_EQ (mStatus.get_multivector()->get_status(value), last);

        // push the frame onto the stack, verify it has indeed one frame, and
        // immediately after unwind it
        stack.push (frame);
        ASSERT_EQ (stack.size (), 1);
        stack.unwind ();

        // and now check the status of this value has been restored to the
        // previous one, and that the stack is empty now
        ASSERT_EQ (mStatus.get_multivector ()->get_status (value), prev);
        ASSERT_EQ (stack.size (), 0);
    }
}

manager<int> mValNbValues;
void handler_val_nbvalues (size_t index, size_t val1, size_t val2) {
    mValNbValues.set_val_nbvalues (index, val1, val2);
}

// Check that restoring the number of feasible values of one variable works
// correctly
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, RestoreValueNbvaluesManager) {

    // first, update the manager with variables and values

    // randomly pick up information for the variables to insert
    vector<string> names;
    vector<vector<value_t<int>>> values;
    randVarIntVals (NB_VARIABLES/10, names, values);

    // now, insert all these variables
    for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

        // add the i-th variable along with its domain
        mValNbValues.add_variable (variable_t (names[j]), values[j]);
    }

    // now, performe the tests
    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty stack
        sstack_t stack;

        // randomly choose one value to update and the new number of feasible mutexes
        size_t value = rand () % (mValNbValues.get_valtable ().size ());
        size_t last = rand () % NB_VALUES;

        // and make a backup copy of the number of feasible mutexes of this
        // value
        size_t prev = mValNbValues.get_valtable ().get_nbvalues (value);

        // first, create an empty frame
        frame_t frame;

        // now, create an action to restore the number of feasible mutexes of
        // the selected value before updating it
        action_t action {handler_val_nbvalues,
            value,
            prev,
            last};
        frame.push (action);

        // Update the number of feasible mutexes of this value. For this, use
        // the handler defined above but reversing the arguments
        handler_val_nbvalues (value, last, prev);

        // verify that the table of values has been properly updated
        ASSERT_EQ (mValNbValues.get_valtable ().get_nbvalues (value), last);

        // push the frame onto the stack, verify it has indeed one frame, and
        // immediately after unwind it
        stack.push (frame);
        ASSERT_EQ (stack.size (), 1);
        stack.unwind ();

        // and now check the number of feasible mutexes of the selected value
        // has been restored to the previous one, and that the stack is empty
        // now
        ASSERT_EQ (mValNbValues.get_valtable ().get_nbvalues (value), prev);
        ASSERT_EQ (stack.size (), 0);
    }
}

manager<int> mFullAssignment;
void handler_var_value_fullassignment (size_t index, size_t val1, size_t val2) {
    mFullAssignment.set_var_value (index, val1, val2);
}
void handler_var_domain_fullassignment (size_t index, size_t val1, size_t val2) {
    mFullAssignment.set_var_nbvalues (index, val1, val2);
}
void handler_val_status_fullassignment (size_t index, size_t val1, size_t val2) {
    mFullAssignment.set_val_status (index, val1, val2);
}
void handler_val_nbvalues_fullassignment (size_t index, size_t val1, size_t val2) {
    mFullAssignment.set_val_nbvalues(index, val1, val2);
}

// Check that int managers are capable of undoing all the actions behind assigning
// a value to a variable
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, UnwindFullAssignIntManager) {

    // first, update the manager with variables and values

    // randomly pick up information for the variables to insert
    vector<string> names;
    vector<vector<value_t<int>>> values;
    randVarIntVals (NB_VARIABLES/10, names, values);

    // now, insert all these variables
    for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

        // add the i-th variable along with its domain
        mFullAssignment.add_variable (variable_t (names[j]), values[j]);
    }

    // randomly choose two different variables making sure they are not the same
    vector<int> variables;
    do {
        variables = randVectorInt (2, NB_VARIABLES/10);
    } while (variables[0] == variables[1]);

    // Add now a constraint (in the form of a lambda function) which returns
    // false if the sum of its arguments is divisible by a number between 2 and
    // 5, over two variables randomly chosen and retrieve the multivector
    quotient = 2 + rand ()%4;
    mFullAssignment.add_constraint([] (int val1, int val2) {
        return (val1 + val2) % quotient != 0;
    }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});
    multivector_t* multivector = mFullAssignment.get_multivector();

    // make a backup copy of the main structs in the manager
    vartable_t vartableFullAssignment (mFullAssignment.get_vartable ());
    valtable_t valtableFullAssignment (mFullAssignment.get_valtable ());
    multivector_t multivectorFullAssignment (*mFullAssignment.get_multivector ());

    // now, performe the tests
    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty stack
        sstack_t stack;

        // VARIABLE <- VALUE
        // --------------------------------------------------------------------

        // modify the value of the first variable randomly selected among those
        // in its domain
        size_t variable = variables[0];
        size_t last = mFullAssignment.get_vartable ().get_first (variable) +
            rand () % (1 + mFullAssignment.get_vartable ().get_last (variable) -
            mFullAssignment.get_vartable ().get_first (variable));

        // and make a backup copy of the current value of the selected variable
        size_t prev = mFullAssignment.get_vartable ().get_value (variable);

        // first, create an empty frame
        frame_t frame;

        // now, create an action to restore the value of the number of values in
        // the domain of the selected variable before updating it
        action_t assign_action {handler_var_value_fullassignment,
            variable,
            prev,
            last};
        frame.push (assign_action);

        // Update the value of this variable and verify that it was properly
        // recorded
        mFullAssignment.get_vartable ().assign (variable, last);
        ASSERT_EQ (mFullAssignment.get_vartable ().get_value (variable), last);

        // STATUS[MUTEX] <- FALSE
        // --------------------------------------------------------------------

        // verify the value randomly selected has a strictly positive number of
        // mutexes. Verify, indeed, the number of enabled mutexes equals its
        // number of mutexes
        ASSERT_TRUE (multivector->get(last).size () > 0);
        ASSERT_EQ (multivector->get(last).size (),
                   mFullAssignment.get_valtable().get_nbvalues (last));

        // count the number of values whose number of enabled mutexes has been
        // updated
        size_t nbvalues_updated = 0;

        // Disable every mutex with the value randomly selected in the domain of
        // the variable raondomly picked up. For each value disabled, update
        // also the number of enabled mutexes of those values mutex with the one
        // being disabled.
        for (auto mvalue : multivector->get(last)) {

            // verify this is enabled by default. Certainly, the same value
            // might be modified in several iterations of this overall loop (the
            // one running over the number of tests to perform), but the point
            // is that after completion of each loop, the state of all values
            // should be restored!
            ASSERT_TRUE (multivector->get_status (mvalue));

            // first, create an action to restore the status of this value
            // before disabling it and push it also onto the frame.
            action_t status_action{handler_val_status_fullassignment,
                size_t(mvalue),
                true,
                false
            };
            frame.push (status_action);

            // and disable this entry and verify that its new status was
            // properly saved
            mFullAssignment.get_multivector()->set_status(mvalue, false);
            ASSERT_FALSE (multivector->get_status (mvalue));

            // NUMBER OF LEGAL VALUES IN EACH DOMAIN
            // ----------------------------------------------------------------

            // next, update the number of legal values in the domain of the
            // variable this value belongs to
            size_t mvariable = mFullAssignment.val_to_var(mvalue);

            // first, create an action to restore the number of legal values in
            // the domain of this variable and push it onto the frame
            action_t domain_action{handler_var_domain_fullassignment,
                mvariable,
                mFullAssignment.get_vartable().get_nbvalues(mvariable),
                mFullAssignment.get_vartable().get_nbvalues(mvariable)-1
            };
            frame.push (domain_action);

            // now, effectively decrement the number of legal values in the
            // domain of this variable and, in passing, verify there is at least
            // one!!
            size_t domain_length = mFullAssignment.get_vartable ().get_nbvalues (mvariable);
            ASSERT_GE (mFullAssignment.get_vartable ().get_nbvalues (mvariable), 1);
            mFullAssignment.get_vartable ().decrement_nbvalues(mvariable);
            ASSERT_EQ (mFullAssignment.get_vartable ().get_nbvalues (mvariable),
                       domain_length-1);

            // NUMBER OF ENABLED MUTEXES
            // ----------------------------------------------------------------

            // in addition, update the number of enabled mutexes of those values
            // which are mutex with this one
            for (auto jvalue : multivector->get (mvalue)) {

                // make a backup copy of the current number of enabled mutexes
                // of the value mvalue
                size_t current_nbvalue = mFullAssignment.get_valtable ().get_nbvalues (jvalue);

                // create an action which restores the current number of enabled
                // values
                action_t nbvalues_action (handler_val_nbvalues_fullassignment,
                                          size_t (jvalue),
                                          mFullAssignment.get_valtable ().get_nbvalues (jvalue),
                                          mFullAssignment.get_valtable ().get_nbvalues (jvalue) - 1);
                frame.push (nbvalues_action);

                // decrement the number of enabled mutexes of one value which is
                // mutex with mvalue
                mFullAssignment.get_valtable ().decrement_nbvalues (jvalue);

                // verify the number of enabled mutexes has been updated
                ASSERT_EQ (mFullAssignment.get_valtable ().get_nbvalues (jvalue),
                           current_nbvalue-1);

                // and increment the number of values whose number of enabled
                // mutexes has been updated
                nbvalues_updated++;
            }
        }

        // once disabling mutexes is over, the number of actions in the frame
        // should be equal to:
        //
        //    1 assign action: assigns a value randomly chosen to a variable
        //                     randomly selected, +
        //
        //    multivector->get (last).size: or the number of mutexes of the
        //                                  value assigned to the variable
        //                                  randomly selected which is strictly
        //                                  equal to the number of values that
        //                                  have been disabled because they are
        //                                  indeed mutex with the value
        //                                  assigned, +
        //
        //    multivector->get (last).size: or the number of mutexes of the
        //                                  value assigned to the variable
        //                                  randomly selected which is strictly
        //                                  equal to the number of updates of
        //                                  the legal number of values in the
        //                                  domain of a variable, +
        //
        //    nbvalues_updated (the number of values whose enabled mutexes has
        //    been updated): values that have updated the number of enabled
        //    mutexes. If a value is disabled then those mutex with it are not
        //    threaten anymore
        ASSERT_EQ (frame.size (), 1+2*multivector->get(last).size ()+nbvalues_updated);

        // before undoing changes, ensure that the main structs of the solver
        // have indeed being modified
        // ASSERT_NE (mFullAssignment.get_vartable (), vartableFullAssignment);
        // ASSERT_NE (mFullAssignment.get_valtable (), valtableFullAssignment);
        // ASSERT_NE (*mFullAssignment.get_multivector (), multivectorFullAssignment);

        // and now undo all modifications after pushing this frame onto the
        // stack. Ensure also the stack only has one frame
        stack.push (frame);
        ASSERT_EQ (stack.size (), 1);
        stack.unwind();

        // now, verify that all changes have been properly unwound. First,
        // ensure the value of the selected variable has been restored
        ASSERT_EQ (mFullAssignment.get_vartable ().get_value (variable), prev);

        // and now verify that all mutexes of the randomly selected value are
        // enabled again along with other information
        for (auto mvalue : multivector->get(last)) {

            // first, ensure that the status of all values that were mutex has
            // been restored
            ASSERT_TRUE (multivector->get_status (mvalue));

            // next, verify also that the number of legal values in the domain
            // of the affected variable has been restored as well
            ASSERT_EQ (mFullAssignment.get_vartable ().get_nbvalues(mFullAssignment.val_to_var(mvalue)),
                vartableFullAssignment.get_nbvalues (mFullAssignment.val_to_var(mvalue)));

            // and now verify also that the number of enabled mutexes of those
            // values which are mutex with mvalue has been restored as well
            for (auto jvalue : multivector->get (mvalue)) {
                ASSERT_EQ (mFullAssignment.get_valtable ().get_nbvalues (jvalue),
                           valtableFullAssignment.get_nbvalues (jvalue));
            }
        }

        // overall, verify the main structs are intact
        // ASSERT_EQ (mFullAssignment.get_vartable (), vartableFullAssignment);
        // ASSERT_EQ (mFullAssignment.get_valtable (), valtableFullAssignment);
        // ASSERT_EQ (*mFullAssignment.get_multivector (), multivectorFullAssignment);
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
