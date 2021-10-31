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
#include<limits>

#include "../fixtures/TSTmanagerfixture.h"

// Define a number of temporal relationships for tests regarding values of type
// time_t
map<int, function<bool (const time_t, const time_t)>> tfuncs = {

// Less than
{0, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) >  0.0;}},

// Less or equal than
{1, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) >= 0.0;}},

// Not equal to
{2, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) != 0.0;}},

// Equal to
{3, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) == 0.0;}},

// Greater or equal than
{4, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) <= 0.0;}},

// Greater than
{5, [] (const time_t val1, const time_t val2)->bool { return difftime (val2, val1) <  0.0;}},
};

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

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create a manager
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // verify now that all variables have been properly initialized. First,
        // check that every value in the domain of each variable has been
        // properly registered in the val table
        const valtable_t<int>& valtable = m.get_valtable ();
        for (int j = 0, idx = 0 ; j < nbvars ; j++ ) {
            for (int k = 0 ; k < values[j].size () ; k++, idx++) {

                // and verify this is the expected value
                ASSERT_EQ (valtable[idx], values[j][k].get_value ());
            }
        }

        // now, verify the contents of the table of variables are also ccorrect
        const vartable_t& vartable = m.get_vartable ();
        for (int j = 0, idx = 0 ; j < nbvars ; idx += values[j++].size ()) {

            // verify the name
            ASSERT_EQ (vartable[j].get_name (), names[j]);

            // verify also that the bounds of the domain have been properly
            // recorded
            ASSERT_EQ (vartable.get_first (j), idx);
            ASSERT_EQ (vartable.get_last (j), idx + values[j].size ()-1);

            // check that the number of plausible values is also correctly
            // stored
            ASSERT_EQ (vartable.get_nbvalues (j), values[j].size ());

            // finally, verify that no variable get a value assigned
            ASSERT_EQ (vartable.get_value (j), string::npos);
        }

        // Before leaving this case, ensure that the multivector is still null
        ASSERT_EQ (m.get_multivector(), nullptr);
    }
}

// Verify the bounds on the domains of all variables in the table of variables
// are contiguous
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ContiguousDomainsManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // check the domains of all variables: check the first value of the
        // first value is strictly equal to 0; next, verify that the last index
        // of the domain of the j-th variable is right before the first index of
        // the domain of the next variable; finally, verify that the last value
        // of the last variable is indeed the index to the last value in the
        // table of values
        const vartable_t& vartable = m.get_vartable ();
        const valtable_t<int>& valtable = m.get_valtable ();
        ASSERT_EQ (vartable.get_first (0), 0);
        for (auto j = 0 ; j < nbvars-1 ; j++ ) {

            // verify the first index to the domain of the next variable is
            // precisely equal to 1 plus the last index of the domain of the
            // current variable
            ASSERT_EQ (1 + vartable.get_last (j),
                       vartable.get_first (j+1));
        }
        ASSERT_EQ (vartable.get_last (nbvars-1),
                   valtable.size ()-1);
    }
}

// Checks that providing domains with repeated values when registering a single
// variable raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, DuplicateValuesInDomainManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // randomly choose a variable
        int variable = rand () % nbvars;

        // and now introduce a duplicate in its domain. For this, randomly
        // choose any value and the position where it is inserted
        int index = rand () % values[variable].size ();
        value_t value = values[variable][rand ()%values[variable].size ()];

        // and insert this existing value into the domain of the variable
        values[variable].insert (index + values[variable].begin (), value);

        // and register this variable. Check that an exception is immediately
        // thrown
        variable_t newvar {names[variable]};
        ASSERT_THROW (m.add_variable (newvar, values[variable]),
                      runtime_error);
    }
}

// Checks that posting constraints on unregistered variables raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, UnregisteredVariablesManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // create two variables, one at least being unregistered. For this,
        // throw a dice and decide among different cases
        variable_t variable0("This variable is not expected to exist!");
        variable_t variable1("This variable is not expected to exist as well!");
        int basecase = rand () % 3;
        switch (basecase) {
            case 1:
                variable1 = variable_t (names[rand () % nbvars]);
                break;
            case 2:
                variable0 = variable_t (names[rand () % nbvars]);
                break;
        }

        // Add now a dummy constraint (in the form of a lambda function which
        // actually does nothing) but invoke it over at least one non-existing
        // variable and verify the operation raises an exception
        ASSERT_THROW(m.add_constraint([] (int val1, int val2) {
            return true;
        }, variable0, variable1),
            invalid_argument);
    }
}

// Checks that conversion from values to variables is correct
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ValToVarManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // run ten executions of this unit test of the same manager
        const vartable_t& vartable = m.get_vartable ();
        const valtable_t<int>& valtable = m.get_valtable ();
        for (auto j = 0 ; j < 10 ; j++) {

            // Whether the mutexes have been properly registered or not is the
            // goal of another unit test. In this case, we are simply going to
            // test that conversion from values to variables is correct.
            // Randomly chose a variable and a value within its domain
            size_t variable = rand () % names.size ();
            size_t value = vartable.get_first (variable) +
                rand () % (1+vartable.get_last(variable) -
                           vartable.get_first (variable));

            // now, check the variable this value belongs to is correct
            ASSERT_EQ (m.val_to_var(value), variable);
            ASSERT_TRUE (vartable.get_first(variable) <= value);
            ASSERT_TRUE (vartable.get_last(variable) >= value);

            // in passing, check also the bound cases
            ASSERT_EQ (m.val_to_var (vartable.get_first (variable)), variable);
            ASSERT_EQ (m.val_to_var (vartable.get_last (variable)), variable);
        }
    }
}

// Checks that constraints properly acknowledge mutexes and that the number of
// enabled mutexes is initially zero, and then matches the number of mutexes set
// up after posting a constraint
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, NbEnabledMutexesManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // verify the number of values is not null
        const valtable_t<int>& valtable = m.get_valtable ();
        ASSERT_TRUE (valtable.size () > 0);

        // before moving further, make sure that the number of enabled mutexes
        // of all values is exactly 0
        for (int j = 0 ; j < valtable.size () ; j++) {
            ASSERT_EQ (valtable.get_nbvalues (j), 0);
        }

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, nbvars);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex.
        // Constraints are defined over two variables randomly selected
        int quotient = 1 + rand ()%6;
        m.add_constraint([quotient] (int val1, int val2)->bool {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[variables[0]]},
            variable_t{names[variables[1]]});

        // once constraints have been added, new mutexes have been discovered
        // (in this case, admittedly, only over two different variables and not
        // all, so that in many cases we'll have 0 mutexes but for the values of
        // the selected variables we'll have an arbitrary number of them). Make
        // sure the number of enabled mutexes is strictly equal to the number of
        // mutexes stored in each value
        const unique_ptr<multivector_t>& multivector = m.get_multivector ();
        for (size_t j = 0 ; j < valtable.size () ; j++) {
            ASSERT_EQ ((*multivector)[j].size (), valtable.get_nbvalues (j));
        }
    }
}

// Check that mutexes can not be defined over the same variable, i.e., func (Xi,
// Xi) raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ReflexiveMutexIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // randomly pick up a variable
        auto variable = rand () % nbvars;

        // and now, verify that invoking a mutex over the same variable
        // immediately generates an exception. The lambda function is
        // irrelevant, but a non-commutative function is intentionally used to
        // provide information about the problem with reflexive mutexes
        ASSERT_THROW (m.add_constraint ([] (int val1, int val2)->bool {
            return val1 < val2;}, variable_t{names[variable]}, variable_t{names[variable]}),
            invalid_argument);
    }
}

// Checks that domains are properly initialized with the same values given when
// variables are registered and precisely the same order
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, CheckDomainIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // gain access to the var and valtables
        const valtable_t<int>& valtable = m.get_valtable();
        const vartable_t& vartable = m.get_vartable();

        // verify the domain of all variables. All values should be registered
        // in the vartable in the same order they were given in the domain and
        // variables should be registered in the same order they were given
        for (int j = 0 ; j < nbvars ; j++) {

            // verify the number of values registered in the vartable equals the
            // size of each domain
            ASSERT_EQ (1 + vartable.get_last (j) - vartable.get_first (j),
                       values[j].size ());

            // for each index in the domain of the j-th variable
            for (size_t idx = vartable.get_first (j); idx <= vartable.get_last (j); idx++) {

                // verify the values are strictly the same
                ASSERT_EQ (values[j][idx-vartable.get_first (j)], valtable[idx]);
            }
        }
    }
}

// Checks that constraints are invoked using the right order of int values. In
// passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintIntManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<int> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, nbvars);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        vector<pair<int, int>> parametersInt;
        m.add_constraint([&parametersInt] (int val1, int val2)->bool {
            parametersInt.push_back (pair<int, int>{val1, val2});
            return true;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        variable_t newvar {"not allowed!"};
        ASSERT_THROW (m.add_variable (newvar, values[0]),
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

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least two are recorded
        vector<string> names;
        vector<vector<value_t<int>>> values;
        int nbvars = 2 + rand () % NB_VARIABLES;
        randVarIntVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<int>(m, names, values);

        // randomly choose two different variables
        auto variables = randVectorInt (2, nbvars, true);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if the sum of its arguments is divisible by a number between 1
        // and 5 ---in case 1 is selected, all combinations are then mutex--- or
        // a very large constant so that no number is selected as a mutex
        int quotient = 1 + rand ()%6;
        m.add_constraint([quotient] (int val1, int val2) {
            if (quotient == 6) {
                return true;
            }
            return (val1 + val2) % quotient != 0;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized
        const valtable_t<int>& valtable = m.get_valtable ();
        const vartable_t& vartable = m.get_vartable ();
        const unique_ptr<multivector_t>& multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<size_t, size_t> bounds1 = get_bounds<int> (variables[0], values);
        pair<size_t, size_t> bounds2 = get_bounds<int> (variables[1], values);

        // to double check that the boundaries have been properly computed,
        // assert their equality
        ASSERT_EQ (vartable.get_first(variables[0]), bounds1.first);
        ASSERT_EQ (vartable.get_last(variables[0]), bounds1.second);
        ASSERT_EQ (vartable.get_first(variables[1]), bounds2.first);
        ASSERT_EQ (vartable.get_last(variables[1]), bounds2.second);

        // and now verify the values of all mutexes
        for (size_t idx1 = vartable.get_first (variables[0]);
             idx1 <= vartable.get_last (variables[0]);
             idx1++) {
                for (size_t idx2 = vartable.get_first (variables[1]);
                     idx2 <= vartable.get_last (variables[1]);
                     idx2++) {

                // get the values stored in the multibitmap
                int num1 = valtable[idx1];
                int num2 = valtable[idx2];

                // verify that those cases where the quotient was randomly
                // selected to be 6 generate no mutexes
                if (quotient==6) {
                    ASSERT_EQ ((*multivector)[idx1].size (), 0);
                } else {

                    // multivectors store mutexes explicitly and thus, if idx2
                    // is not a mutex of idx1 it is just not present in its
                    // vector of mutexes
                    ASSERT_TRUE ( ( multivector->find (idx1, idx2) && (num1 + num2) % quotient == 0 ) ||
                                  (!multivector->find (idx1, idx2) && (num1 + num2) % quotient != 0));

                    // Mutexes are reflective, thus verify the opposite as well
                    ASSERT_TRUE ( ( multivector->find (idx2, idx1) && (num1 + num2) % quotient == 0 ) ||
                                  (!multivector->find (idx2, idx1) && (num1 + num2) % quotient != 0));
                }
            }
        }
    }
}

// Check that mutexes can not be defined over the same variable, i.e., func (Xi,
// Xi) raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ReflexiveMutexStringManager) {

    for (auto i = 0 ; i < NB_TESTS/100; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<string> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<string>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarStringVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<string>(m, names, values);

        // randomly pick up a variable
        auto variable = rand () % nbvars;

        // and now, verify that invoking a mutex over the same variable
        // immediately generates an exception. The lambda function is
        // irrelevant, but a non-commutative function is intentionally used to
        // provide information about the problem with reflexive mutexes
        ASSERT_THROW (m.add_constraint ([] (string val1, string val2)->bool {
            return val1 < val2;}, variable_t{names[variable]}, variable_t{names[variable]}),
            invalid_argument);
    }
}

// Checks that domains are properly initialized with the same values given when
// variables are registered and precisely the same order
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, CheckDomainStringManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<string> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<string>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarStringVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<string>(m, names, values);

        // gain access to the var and valtables
        const valtable_t<string>& valtable = m.get_valtable();
        const vartable_t& vartable = m.get_vartable();

        // verify the domain of all variables. All values should be registered
        // in the vartable in the same order they were given in the domain and
        // variables should be registered in the same order they were given
        for (int j = 0 ; j < nbvars ; j++) {

            // verify the number of values registered in the vartable equals the
            // size of each domain
            ASSERT_EQ (1 + vartable.get_last (j) - vartable.get_first (j),
                       values[j].size ());

            // for each index in the domain of the j-th variable
            for (size_t idx = vartable.get_first (j); idx <= vartable.get_last (j); idx++) {

                // verify the values are strictly the same
                ASSERT_EQ (values[j][idx-vartable.get_first (j)], valtable[idx]);
            }
        }
    }
}

// Checks that constraints are invoked using the right order of string values.
// In passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintStringManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<string> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<string>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarStringVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<string>(m, names, values);

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, nbvars);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        vector<pair<string, string>> parametersString;
        m.add_constraint([&parametersString] (string val1, string val2) {
            parametersString.push_back (pair<string, string>{val1, val2});
            return true;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        variable_t newvar {"not allowed!"};
        ASSERT_THROW (m.add_variable (newvar, values[0]),
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

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least two are recorded
        vector<string> names;
        vector<vector<value_t<string>>> values;
        int nbvars = 2 + rand () % NB_VARIABLES;
        randVarStringVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<string>(m, names, values);

        // randomly choose two different variables
        auto variables = randVectorInt (2, nbvars, true);

        // Add now a constraint (in the form of a lambda function) which returns
        // false if any of the strings contains a letter randomly selected
        string chr = randString (1);
        m.add_constraint([chr] (string val1, string val2) {
            return !val1.find (chr) && !val2.find (chr);
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized
        const valtable_t<string>& valtable = m.get_valtable ();
        const vartable_t& vartable = m.get_vartable ();
        const unique_ptr<multivector_t>& multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<size_t, size_t> bounds1 = get_bounds<string> (variables[0], values);
        pair<size_t, size_t> bounds2 = get_bounds<string> (variables[1], values);

        // to double check that the boundaries have been properly computed,
        // assert their equality
        ASSERT_EQ (vartable.get_first(variables[0]), bounds1.first);
        ASSERT_EQ (vartable.get_last(variables[0]), bounds1.second);
        ASSERT_EQ (vartable.get_first(variables[1]), bounds2.first);
        ASSERT_EQ (vartable.get_last(variables[1]), bounds2.second);

        // and now verify the values of all mutexes
        for (size_t idx1 = vartable.get_first (variables[0]);
             idx1 <= vartable.get_last (variables[0]);
             idx1++) {
                for (size_t idx2 = vartable.get_first (variables[1]);
                     idx2 <= vartable.get_last (variables[1]);
                     idx2++) {

                // get the values stored in the multibitmap
                string str1 = valtable[idx1];
                string str2 = valtable[idx2];

                // and verify that only those cases where either argument
                // contains the letter randomly selected is recognized as a
                // mutex. Note that multivectors store mutexes explicitly
                ASSERT_TRUE ( (multivector->find (idx1, idx2) && (str1.find (chr) || str2.find (chr))) ||
                              (!multivector->find (idx1, idx2) && !str1.find (chr) && !str2.find (chr) ));

                // Mutexes are reflective, thus verify the opposite as well
                ASSERT_TRUE ( (multivector->find (idx2, idx1) && (str1.find (chr) || str2.find (chr))) ||
                              (!multivector->find (idx2, idx1) && !str1.find (chr) && !str2.find (chr) ));
            }
        }
    }
}

// Check that mutexes can not be defined over the same variable, i.e., func (Xi,
// Xi) raises an exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, ReflexiveMutexTimeManager) {

    for (auto i = 0 ; i < NB_TESTS/100; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<time_t> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarTimeVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<time_t>(m, names, values);

        // randomly pick up a variable
        auto variable = rand () % nbvars;

        // and now, verify that invoking a mutex over the same variable
        // immediately generates an exception. The lambda function is
        // irrelevant, but a non-commutative function is intentionally used to
        // provide information about the problem with reflexive mutexes
        ASSERT_THROW (m.add_constraint ([] (time_t val1, time_t val2)->bool {
            return val1 < val2;}, variable_t{names[variable]}, variable_t{names[variable]}),
            invalid_argument);
    }
}

// Checks that domains are properly initialized with the same values given when
// variables are registered and precisely the same order
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, CheckDomainTimeManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<time_t> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarTimeVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<time_t>(m, names, values);

        // gain access to the var and valtables
        const valtable_t<time_t>& valtable = m.get_valtable();
        const vartable_t& vartable = m.get_vartable();

        // verify the domain of all variables. All values should be registered
        // in the vartable in the same order they were given in the domain and
        // variables should be registered in the same order they were given
        for (int j = 0 ; j < nbvars ; j++) {

            // verify the number of values registered in the vartable equals the
            // size of each domain
            ASSERT_EQ (1 + vartable.get_last (j) - vartable.get_first (j),
                       values[j].size ());

            // for each index in the domain of the j-th variable
            for (size_t idx = vartable.get_first (j); idx <= vartable.get_last (j); idx++) {

                // verify the values are strictly the same
                ASSERT_EQ (values[j][idx-vartable.get_first (j)], valtable[idx]);
            }
        }
    }
}

// Checks that constraints are invoked using the right order of time values.
// In passing, verify that adding variables after posting constraints raises an
// exception
// ----------------------------------------------------------------------------
TEST_F (ManagerFixture, InvokeConstraintTimeManager) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        manager<time_t> m;

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least one is recorded
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        int nbvars = 1 + rand () % NB_VARIABLES;
        randVarTimeVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<time_t>(m, names, values);

        // randomly choose two different variables (which could be the same ...)
        auto variables = randVectorInt (2, nbvars);

        // Add now a constraint (in the form of a lambda function) which stores
        // all combinations of values as pairs
        vector<pair<time_t, time_t>> parametersTime;
        m.add_constraint([&parametersTime] (time_t val1, time_t val2) {
            parametersTime.push_back (pair<time_t, time_t>{val1, val2});
            return true;
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // before making the important assertion, ensure also that after posting
        // constraints, the multivector is not null anymore
        ASSERT_NE (m.get_multivector(), nullptr);

        // and that posting additional constraints automatically raises an
        // exception
        variable_t newvar {"not allowed!"};
        ASSERT_THROW (m.add_variable (newvar, values[0]),
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

        // randomly pick up information for all variables to insert. The number
        // of variables to insert is randomly selected and it is guaranteed, at
        // least two are recorded
        vector<string> names;
        vector<vector<value_t<time_t>>> values;
        int nbvars = 2 + rand () % NB_VARIABLES;
        randVarTimeVals (nbvars, names, values);

        // and add all these variables to the manager
        addVariables<time_t>(m, names, values);

        // randomly choose two different variables
        auto variables = randVectorInt (2, nbvars, true);

        // Add now a constraint (in the form of a lambda function) which
        // verifies whether one among six different plausible temporal
        // relationships holds between the values of the variables randomly
        // selected. Note the relational operators == and != intentionally
        // create either no mutex or so many.
        int op = rand () % 6;
        m.add_constraint ([op] (const time_t val1, const time_t val2)->bool {
            return tfuncs[op] (val1, val2);
        }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});

        // now, process all values of both variables and verify that mutexes
        // have been properly recognized
        const valtable_t<time_t>& valtable = m.get_valtable ();
        const vartable_t& vartable = m.get_vartable ();
        const unique_ptr<multivector_t>& multivector = m.get_multivector();

        // get the index to the first and last value in the domain of each
        // variable
        pair<size_t, size_t> bounds1 = get_bounds<time_t> (variables[0], values);
        pair<size_t, size_t> bounds2 = get_bounds<time_t> (variables[1], values);

        // to double check that the boundaries have been properly computed,
        // assert their equality
        ASSERT_EQ (vartable.get_first(variables[0]), bounds1.first);
        ASSERT_EQ (vartable.get_last(variables[0]), bounds1.second);
        ASSERT_EQ (vartable.get_first(variables[1]), bounds2.first);
        ASSERT_EQ (vartable.get_last(variables[1]), bounds2.second);

        // and now verify the values of all mutexes
        for (size_t idx1 = vartable.get_first (variables[0]);
             idx1 <= vartable.get_last (variables[0]);
             idx1++) {
                for (size_t idx2 = vartable.get_first (variables[1]);
                     idx2 <= vartable.get_last (variables[1]);
                     idx2++) {

                // get the values stored in the multibitmap
                time_t num1 = valtable[idx1];
                time_t num2 = valtable[idx2];

                // verify now that all mutexes have been properly stored
                ASSERT_TRUE ( ( multivector->find (idx1, idx2) && !tfuncs[op] (num1, num2)) ||
                              (!multivector->find (idx1, idx2) &&  tfuncs[op] (num1, num2)) );

                ASSERT_TRUE ( ( multivector->find (idx2, idx1) && !tfuncs[op] (num1, num2)) ||
                              (!multivector->find (idx2, idx1) &&  tfuncs[op] (num1, num2)) );
            }
        }
    }
}

// manager<int> mVarNbValues;
// void handler_var_nbvalues (size_t index, size_t val1, size_t val2) {
//     mVarNbValues.set_var_nbvalues (index, val1, val2);
// }

// // Check that restoring the number of values in the domain of a variable works
// // correctly
// // ----------------------------------------------------------------------------
// TEST_F (ManagerFixture, RestoreVariableNbvaluesManager) {

//     // first, update the manager with variables and values

//     // randomly pick up information for the variables to insert
//     vector<string> names;
//     vector<vector<value_t<int>>> values;
//     randVarIntVals (NB_VARIABLES/10, names, values);

//     // now, insert all these variables
//     for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

//         // add the i-th variable along with its domain
//         mVarNbValues.add_variable (variable_t (names[j]), values[j]);
//     }

//     // now, performe the tests
//     for (auto i = 0 ; i < NB_TESTS ; i++) {

//         // create an empty stack
//         sstack_t stack;

//         // randomly choose one variable to update and the new value
//         size_t variable = rand () % (NB_VARIABLES/10);
//         size_t last = rand () % NB_VALUES;

//         // and make a backup copy of the number of values in the domain of the
//         // selected variable
//         size_t prev = mVarNbValues.get_vartable ().get_nbvalues (variable);

//         // first, create an empty frame
//         frame_t frame;

//         // now, create an action to restore the value of the number of values in
//         // the domain of the selected variable before updating it
//         action_t action {handler_var_nbvalues,
//             variable,
//             prev,
//             last};
//         frame.push (action);

//         // Update the number of feasible values. For this, use the handler
//         // defined above but reversing the arguments
//         handler_var_nbvalues (variable, last, prev);

//         // verify that the table of variables has been properly updated
//         ASSERT_EQ (mVarNbValues.get_vartable().get_nbvalues(variable), last);

//         // push the frame onto the stack, verify it has indeed one frame, and
//         // immediately after unwind it
//         stack.push (frame);
//         ASSERT_EQ (stack.size (), 1);
//         stack.unwind ();

//         // and now check the number of values in the domain of this variable has
//         // been restored to the previous one, and that the stack is empty now
//         ASSERT_EQ (mVarNbValues.get_vartable ().get_nbvalues (variable), prev);
//         ASSERT_EQ (stack.size (), 0);
//     }
// }

// manager<int> mValue;
// void handler_var_value (size_t index, size_t val1, size_t val2) {
//     mValue.set_var_value (index, val1, val2);
// }

// // Check that restoring the number of values in the domain of a variable works
// // correctly
// // ----------------------------------------------------------------------------
// TEST_F (ManagerFixture, RestoreVariableValueManager) {

//     // first, update the manager with variables and values

//     // randomly pick up information for the variables to insert
//     vector<string> names;
//     vector<vector<value_t<int>>> values;
//     randVarIntVals (NB_VARIABLES/10, names, values);

//     // now, insert all these variables
//     for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

//         // add the i-th variable along with its domain
//         mValue.add_variable (variable_t (names[j]), values[j]);
//     }

//     // now, performe the tests
//     for (auto i = 0 ; i < NB_TESTS ; i++) {

//         // create an empty stack
//         sstack_t stack;

//         // randomly choose one variable to update and the new value
//         size_t variable = rand () % (NB_VARIABLES/10);
//         size_t last = rand () % NB_VALUES;

//         // and make a backup copy of the current value of the selected variable
//         size_t prev = mValue.get_vartable ().get_value (variable);

//         // first, create an empty frame
//         frame_t frame;

//         // now, create an action to restore the value of the selected variable
//         // before updating it
//         action_t action {handler_var_value,
//             variable,
//             prev,
//             last};
//         frame.push (action);

//         // Update the value of this variable. For this, use the handler defined
//         // above but reversing the arguments
//         handler_var_value (variable, last, prev);

//         // verify that the table of variables has been properly updated
//         ASSERT_EQ (mValue.get_vartable().get_value(variable), last);

//         // push the frame onto the stack, verify it has indeed one frame, and
//         // immediately after unwind it
//         stack.push (frame);
//         ASSERT_EQ (stack.size (), 1);
//         stack.unwind ();

//         // and now check the value assigned to this variable has been restored
//         // to the previous one, and that the stack is empty now
//         ASSERT_EQ (mValue.get_vartable ().get_value (variable), prev);
//         ASSERT_EQ (stack.size (), 0);
//     }
// }

// manager<int> mStatus;
// void handler_val_status (size_t index, size_t val1, size_t val2) {
//     mStatus.set_val_status (index, val1, val2);
// }

// // Check that restoring the number of values in the domain of a variable works
// // correctly
// // ----------------------------------------------------------------------------
// TEST_F (ManagerFixture, RestoreValueStatusManager) {

//     // first, update the manager with variables and values

//     // randomly pick up information for the variables to insert
//     vector<string> names;
//     vector<vector<value_t<int>>> values;
//     randVarIntVals (NB_VARIABLES/10, names, values);

//     // now, insert all these variables
//     for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

//         // add the i-th variable along with its domain
//         mStatus.add_variable (variable_t (names[j]), values[j]);
//     }

//     // for the table of values to be initialized it is mandatory to post
//     // constraints. The following post a dummy constraint to the first two
//     // variables
//     mStatus.add_constraint([] (int val1, int val2) {
//         return true;
//     }, variable_t{names[0]}, variable_t{names[1]});
//     ASSERT_NE (mStatus.get_multivector (), nullptr);

//     // now, performe the tests
//     for (auto i = 0 ; i < NB_TESTS ; i++) {

//         // create an empty stack
//         sstack_t stack;

//         // randomly choose one value to update and the new status
//         size_t value = rand () % mStatus.get_multivector ()->size ();
//         size_t last = 0;

//         // and make a backup copy of the current status of the selected value
//         size_t prev = mStatus.get_multivector ()->get_status (value);

//         // in passing ensure that all values are enabled by default
//         ASSERT_EQ (prev, 1);

//         // first, create an empty frame
//         frame_t frame;

//         // now, create an action to restore the status of the selected value
//         // before updating it
//         action_t action {handler_val_status,
//             value,
//             prev,
//             last};
//         frame.push (action);

//         // Update the status of this value. For this, use the handler defined
//         // above but reversing the arguments
//         handler_val_status (value, last, prev);

//         // verify that the table of variables has been properly updated
//         ASSERT_EQ (mStatus.get_multivector()->get_status(value), last);

//         // push the frame onto the stack, verify it has indeed one frame, and
//         // immediately after unwind it
//         stack.push (frame);
//         ASSERT_EQ (stack.size (), 1);
//         stack.unwind ();

//         // and now check the status of this value has been restored to the
//         // previous one, and that the stack is empty now
//         ASSERT_EQ (mStatus.get_multivector ()->get_status (value), prev);
//         ASSERT_EQ (stack.size (), 0);
//     }
// }

// manager<int> mValNbValues;
// void handler_val_nbvalues (size_t index, size_t val1, size_t val2) {
//     mValNbValues.set_val_nbvalues (index, val1, val2);
// }

// // Check that restoring the number of feasible values of one variable works
// // correctly
// // ----------------------------------------------------------------------------
// TEST_F (ManagerFixture, RestoreValueNbvaluesManager) {

//     // first, update the manager with variables and values

//     // randomly pick up information for the variables to insert
//     vector<string> names;
//     vector<vector<value_t<int>>> values;
//     randVarIntVals (NB_VARIABLES/10, names, values);

//     // now, insert all these variables
//     for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

//         // add the i-th variable along with its domain
//         mValNbValues.add_variable (variable_t (names[j]), values[j]);
//     }

//     // now, performe the tests
//     for (auto i = 0 ; i < NB_TESTS ; i++) {

//         // create an empty stack
//         sstack_t stack;

//         // randomly choose one value to update and the new number of feasible mutexes
//         size_t value = rand () % (mValNbValues.get_valtable ().size ());
//         size_t last = rand () % NB_VALUES;

//         // and make a backup copy of the number of feasible mutexes of this
//         // value
//         size_t prev = mValNbValues.get_valtable ().get_nbvalues (value);

//         // first, create an empty frame
//         frame_t frame;

//         // now, create an action to restore the number of feasible mutexes of
//         // the selected value before updating it
//         action_t action {handler_val_nbvalues,
//             value,
//             prev,
//             last};
//         frame.push (action);

//         // Update the number of feasible mutexes of this value. For this, use
//         // the handler defined above but reversing the arguments
//         handler_val_nbvalues (value, last, prev);

//         // verify that the table of values has been properly updated
//         ASSERT_EQ (mValNbValues.get_valtable ().get_nbvalues (value), last);

//         // push the frame onto the stack, verify it has indeed one frame, and
//         // immediately after unwind it
//         stack.push (frame);
//         ASSERT_EQ (stack.size (), 1);
//         stack.unwind ();

//         // and now check the number of feasible mutexes of the selected value
//         // has been restored to the previous one, and that the stack is empty
//         // now
//         ASSERT_EQ (mValNbValues.get_valtable ().get_nbvalues (value), prev);
//         ASSERT_EQ (stack.size (), 0);
//     }
// }

// manager<int> mFullAssignment;
// void handler_var_value_fullassignment (size_t index, size_t val1, size_t val2) {
//     mFullAssignment.set_var_value (index, val1, val2);
// }
// void handler_var_domain_fullassignment (size_t index, size_t val1, size_t val2) {
//     mFullAssignment.set_var_nbvalues (index, val1, val2);
// }
// void handler_val_status_fullassignment (size_t index, size_t val1, size_t val2) {
//     mFullAssignment.set_val_status (index, val1, val2);
// }
// void handler_val_nbvalues_fullassignment (size_t index, size_t val1, size_t val2) {
//     mFullAssignment.set_val_nbvalues(index, val1, val2);
// }

// // Check that int managers are capable of undoing all the actions behind assigning
// // a value to a variable
// // ----------------------------------------------------------------------------
// TEST_F (ManagerFixture, UnwindFullAssignIntManager) {

//     // first, update the manager with variables and values

//     // randomly pick up information for the variables to insert
//     vector<string> names;
//     vector<vector<value_t<int>>> values;
//     randVarIntVals (NB_VARIABLES/10, names, values);

//     // now, insert all these variables
//     for (int j = 0 ; j < NB_VARIABLES/10 ; j++) {

//         // add the i-th variable along with its domain
//         mFullAssignment.add_variable (variable_t (names[j]), values[j]);
//     }

//     // randomly choose two different variables making sure they are not the same
//     vector<int> variables;
//     do {
//         variables = randVectorInt (2, NB_VARIABLES/10);
//     } while (variables[0] == variables[1]);

//     // Add now a constraint (in the form of a lambda function) which returns
//     // false if the sum of its arguments is divisible by a number between 2 and
//     // 5, over two variables randomly chosen and retrieve the multivector
//     quotient = 2 + (rand ()%4);
//     mFullAssignment.add_constraint([] (int val1, int val2) {
//         return (val1 + val2) % quotient != 0;
//     }, variable_t{names[variables[0]]}, variable_t{names[variables[1]]});
//     multivector_t* multivector = mFullAssignment.get_multivector();

//     // make a backup copy of the main structs in the manager
//     vartable_t vartableFullAssignment (mFullAssignment.get_vartable ());
//     valtable_t valtableFullAssignment (mFullAssignment.get_valtable ());
//     // multivector_t multivectorFullAssignment (*mFullAssignment.get_multivector ());

//     // now, performe the tests
//     for (auto i = 0 ; i < NB_TESTS ; i++) {

//         // create an empty stack
//         sstack_t stack;

//         // VARIABLE <- VALUE
//         // --------------------------------------------------------------------

//         // modify the value of the first variable randomly selected among those
//         // in its domain
//         auto randidx = rand ()%2;
//         size_t variable = variables[randidx];
//         size_t last = mFullAssignment.get_vartable ().get_first (variable) +
//             rand () % (1 + mFullAssignment.get_vartable ().get_last (variable) -
//                        mFullAssignment.get_vartable ().get_first (variable));

//         // and make a backup copy of the current value of the selected variable
//         size_t prev = mFullAssignment.get_vartable ().get_value (variable);

//         // first, create an empty frame
//         frame_t frame;

//         // now, create an action to restore the value of the number of values in
//         // the domain of the selected variable before updating it
//         action_t assign_action {handler_var_value_fullassignment,
//             variable,
//             prev,
//             last};
//         frame.push (assign_action);

//         // Update the value of this variable and verify that it was properly
//         // recorded
//         mFullAssignment.get_vartable ().assign (variable, last);
//         ASSERT_EQ (mFullAssignment.get_vartable ().get_value (variable), last);

//         // STATUS[MUTEX] <- FALSE
//         // --------------------------------------------------------------------

//         // verify the number of enabled mutexes equals its number of mutexes.
//         // Verify, indeed the value randomly selected has a strictly positive
//         // number of mutexes. Although very unlikely it is possible to have a
//         // null number of mutexes. If this is the case, strictly verify that
//         // this is correct
//         if ((*multivector)[last].size () == 0) {

//             auto lastvalue = mFullAssignment.get_valtable().get_value(last).get_value ();
//             cout << " Variable idx  : " << variables[randidx] << endl;
//             cout << " \t 1st value  : " << mFullAssignment.get_vartable ().get_first (variable) << endl;
//             cout << " \t lst value  : " << mFullAssignment.get_vartable ().get_last (variable) << endl;
//             cout << " 1-Variable idx: " << variables[1-randidx] << endl;
//             cout << " \t 1st value  : " << mFullAssignment.get_vartable ().get_first (variables[1-randidx]) << endl;
//             cout << " \t lst value  : " << mFullAssignment.get_vartable ().get_last (variables[1-randidx]) << endl;
//             cout << " Quotient      : " << quotient << endl;
//             cout << " Nuevo índice  : " << last << endl;
//             cout << " Nuevo valor   : " << lastvalue << endl;
//             for (auto kidx = mFullAssignment.get_vartable ().get_first(1-randidx) ;
//                  kidx <= mFullAssignment.get_vartable ().get_last (1-randidx) ;
//                  kidx++) {
//                 cout << "\t" << lastvalue << " + " << mFullAssignment.get_valtable().get_value (kidx).get_value () << ": " << lastvalue + mFullAssignment.get_valtable().get_value (kidx).get_value () << endl;
//                 cout << "\t\t remainder: " << (lastvalue + mFullAssignment.get_valtable().get_value (kidx).get_value ()) % quotient << endl;
//             }
//         }
//         ASSERT_EQ ((*multivector)[last].size (),
//                    mFullAssignment.get_valtable().get_nbvalues (last));
//         ASSERT_TRUE ((*multivector)[last].size () > 0);

//         // count the number of values whose number of enabled mutexes has been
//         // updated
//         size_t nbvalues_updated = 0;

//         // Disable every mutex with the value randomly selected in the domain of
//         // the variable raondomly picked up. For each value disabled, update
//         // also the number of enabled mutexes of those values mutex with the one
//         // being disabled.
//         for (auto mvalue : (*multivector)[last]) {

//             // verify this is enabled by default. Certainly, the same value
//             // might be modified in several iterations of this overall loop (the
//             // one running over the number of tests to perform), but the point
//             // is that after completion of each loop, the state of all values
//             // should be restored!
//             ASSERT_TRUE (multivector->get_status (mvalue));

//             // first, create an action to restore the status of this value
//             // before disabling it and push it also onto the frame.
//             action_t status_action{handler_val_status_fullassignment,
//                 size_t(mvalue),
//                 true,
//                 false
//             };
//             frame.push (status_action);

//             // and disable this entry and verify that its new status was
//             // properly saved
//             mFullAssignment.get_multivector()->set_status(mvalue, false);
//             ASSERT_FALSE (multivector->get_status (mvalue));

//             // NUMBER OF LEGAL VALUES IN EACH DOMAIN
//             // ----------------------------------------------------------------

//             // next, update the number of legal values in the domain of the
//             // variable this value belongs to
//             size_t mvariable = mFullAssignment.val_to_var(mvalue);

//             // first, create an action to restore the number of legal values in
//             // the domain of this variable and push it onto the frame
//             action_t domain_action{handler_var_domain_fullassignment,
//                 mvariable,
//                 mFullAssignment.get_vartable().get_nbvalues(mvariable),
//                 mFullAssignment.get_vartable().get_nbvalues(mvariable)-1
//             };
//             frame.push (domain_action);

//             // now, effectively decrement the number of legal values in the
//             // domain of this variable and, in passing, verify there is at least
//             // one!!
//             size_t domain_length = mFullAssignment.get_vartable ().get_nbvalues (mvariable);
//             ASSERT_GE (mFullAssignment.get_vartable ().get_nbvalues (mvariable), 1);
//             mFullAssignment.get_vartable ().decrement_nbvalues(mvariable);
//             ASSERT_EQ (mFullAssignment.get_vartable ().get_nbvalues (mvariable),
//                        domain_length-1);

//             // NUMBER OF ENABLED MUTEXES
//             // ----------------------------------------------------------------

//             // in addition, update the number of enabled mutexes of those values
//             // which are mutex with this one
//             for (auto jvalue : (*multivector)[mvalue]) {

//                 // make a backup copy of the current number of enabled mutexes
//                 // of the value mvalue
//                 size_t current_nbvalue = mFullAssignment.get_valtable ().get_nbvalues (jvalue);

//                 // create an action which restores the current number of enabled
//                 // values
//                 action_t nbvalues_action (handler_val_nbvalues_fullassignment,
//                                           size_t (jvalue),
//                                           mFullAssignment.get_valtable ().get_nbvalues (jvalue),
//                                           mFullAssignment.get_valtable ().get_nbvalues (jvalue) - 1);
//                 frame.push (nbvalues_action);

//                 // decrement the number of enabled mutexes of one value which is
//                 // mutex with mvalue
//                 mFullAssignment.get_valtable ().decrement_nbvalues (jvalue);

//                 // verify the number of enabled mutexes has been updated
//                 ASSERT_EQ (mFullAssignment.get_valtable ().get_nbvalues (jvalue),
//                            current_nbvalue-1);

//                 // and increment the number of values whose number of enabled
//                 // mutexes has been updated
//                 nbvalues_updated++;
//             }
//         }

//         // once disabling mutexes is over, the number of actions in the frame
//         // should be equal to:
//         //
//         //    1 assign action: assigns a value randomly chosen to a variable
//         //                     randomly selected, +
//         //
//         //    multivector->get (last).size: or the number of mutexes of the
//         //                                  value assigned to the variable
//         //                                  randomly selected which is strictly
//         //                                  equal to the number of values that
//         //                                  have been disabled because they are
//         //                                  indeed mutex with the value
//         //                                  assigned, +
//         //
//         //    multivector->get (last).size: or the number of mutexes of the
//         //                                  value assigned to the variable
//         //                                  randomly selected which is strictly
//         //                                  equal to the number of updates of
//         //                                  the legal number of values in the
//         //                                  domain of a variable, +
//         //
//         //    nbvalues_updated (the number of values whose enabled mutexes has
//         //    been updated): values that have updated the number of enabled
//         //    mutexes. If a value is disabled then those mutex with it are not
//         //    threaten anymore
//         ASSERT_EQ (frame.size (), 1+2*(*multivector)[last].size ()+nbvalues_updated);

//         // before undoing changes, ensure that the main structs of the solver
//         // have indeed being modified
//         // ASSERT_NE (mFullAssignment.get_vartable (), vartableFullAssignment);
//         // ASSERT_NE (mFullAssignment.get_valtable (), valtableFullAssignment);
//         // ASSERT_NE (*mFullAssignment.get_multivector (), multivectorFullAssignment);

//         // and now undo all modifications after pushing this frame onto the
//         // stack. Ensure also the stack only has one frame
//         stack.push (frame);
//         ASSERT_EQ (stack.size (), 1);
//         stack.unwind();

//         // now, verify that all changes have been properly unwound. First,
//         // ensure the value of the selected variable has been restored
//         ASSERT_EQ (mFullAssignment.get_vartable ().get_value (variable), prev);

//         // and now verify that all mutexes of the randomly selected value are
//         // enabled again along with other information
//         for (auto mvalue : (*multivector)[last]) {

//             // first, ensure that the status of all values that were mutex has
//             // been restored
//             ASSERT_TRUE (multivector->get_status (mvalue));

//             // next, verify also that the number of legal values in the domain
//             // of the affected variable has been restored as well
//             ASSERT_EQ (mFullAssignment.get_vartable ().get_nbvalues(mFullAssignment.val_to_var(mvalue)),
//                 vartableFullAssignment.get_nbvalues (mFullAssignment.val_to_var(mvalue)));

//             // and now verify also that the number of enabled mutexes of those
//             // values which are mutex with mvalue has been restored as well
//             for (auto jvalue : (*multivector)[mvalue]) {
//                 ASSERT_EQ (mFullAssignment.get_valtable ().get_nbvalues (jvalue),
//                            valtableFullAssignment.get_nbvalues (jvalue));
//             }
//         }

//         // overall, verify the main structs are intact
//         // ASSERT_EQ (mFullAssignment.get_vartable (), vartableFullAssignment);
//         // ASSERT_EQ (mFullAssignment.get_valtable (), valtableFullAssignment);
//         // ASSERT_EQ (*mFullAssignment.get_multivector (), multivectorFullAssignment);
//     }
// }


// Local Variables:
// mode:cpp
// fill-column:80
// End:
