// -*- coding: utf-8 -*-
// TSTvartable_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mar 10-08-2021 00:27:24.733744068 (1628548044)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of tables of CSP variables

#include "../fixtures/TSTvartablefixture.h"

// Checks the creation of a table of CSP variables generates an empty table
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, EmptyVartable) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and verify it consists of precisely 0 elements
        ASSERT_EQ (vartable.size (), 0);
    }
}

// Checks the population of a table of CSP variables is properly initialized
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, InitVartable) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and now populate it with up to NB_VARIABLES and their domains
        vector<string> names;
        vector<pair<int, int>> indices;
        populate (vartable, NB_VARIABLES, NB_VALUES, names, indices);

        // Now, traverse the whole table of CSP variables, making sure that
        // variables and their domain have been properly stored, and that the
        // number of plausible values and the value assigned to it have been
        // correctly initialized
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {

            // Verify the name of the variable
            ASSERT_EQ (vartable.get_variable (j).get_name (),
                       names[j]);

            // and also the limits of its domain
            ASSERT_EQ (vartable.get_first (j),
                       indices[j].first);
            ASSERT_EQ (vartable.get_last (j),
                       indices[j].second);

            // verify the size of the domain has been correctly initialized
            ASSERT_EQ (vartable.get_nbvalues (j),
                       1 + indices[j].second - indices[j].first);

            // and that no value is initially assigned
            ASSERT_EQ (vartable.get_value (j), -1);
        }
    }
}

// Checks that decrementing the number of plausible values works as expected
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, DecrementVartable) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and now populate it with up to NB_VARIABLES and their domains
        vector<string> names;
        vector<pair<int, int>> indices;
        populate (vartable, NB_VARIABLES, NB_VALUES, names, indices);

        // Now, traverse the whole table of CSP variables, decrementing the
        // number of plausible values in each one and verifying that the result
        // is the expected one
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {

            // verify that decrementing the number of plausible values produces
            // the expected result
            auto result = vartable.decrement_nbvalues(j);

            // On one hand, the function returns the new number of plausible
            // values, and this number should be equal to the number reported by
            // the table
            ASSERT_EQ (vartable.get_nbvalues(j), result);

            // on the other hand, this number should be strictly equal to the
            // value initialized minus 1: 1 + last - finst - 1 = last - first
            ASSERT_EQ (vartable.get_last (j) - vartable.get_first (j), result);
        }
    }
}

// Checks that incrementing the number of plausible values works as expected
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, IncrementVartable) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and now populate it with up to NB_VARIABLES and their domains
        vector<string> names;
        vector<pair<int, int>> indices;
        populate (vartable, NB_VARIABLES, NB_VALUES, names, indices);

        // Now, traverse the whole table of CSP variables, incrementing the
        // number of plausible values in each one and verifying that the result
        // is the expected one
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {

            // verify that incrementing the number of plausible values produces
            // the expected result
            auto result = vartable.increment_nbvalues(j);

            // On one hand, the function returns the new number of plausible
            // values, and this number should be equal to the number reported by
            // the table
            ASSERT_EQ (vartable.get_nbvalues(j), result);

            // on the other hand, this number should be strictly equal to the
            // value initialized plus 1: 1 + last - finst + 1 = 2 + last -
            // first. Note this value is strictly impossible, but the table of
            // variables is not supposed to perform any checks
            ASSERT_EQ (2 + vartable.get_last (j) - vartable.get_first (j), result);
        }
    }
}

// Checks that setting the number of plausible values works as expected
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, SettingVartable) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and now populate it with up to NB_VARIABLES and their domains
        vector<string> names;
        vector<pair<int, int>> indices;
        populate (vartable, NB_VARIABLES, NB_VALUES, names, indices);

        // randomly pick up up to NB_VARIABLES random numbers
        vector<int> nb_values = randVectorInt (NB_VARIABLES, NB_VALUES);

        // Now, traverse the whole table of CSP variables, setting the number of
        // plausible values in each one to a randome value and verifying that
        // the result is the expected one
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {

            // set the number of plausible values
            auto result = vartable.set_nbvalues(j, nb_values[j]);

            // Setting the number of plausible values should return the new
            // number of plausible values
            ASSERT_EQ (vartable.get_nbvalues(j), result);
        }
    }
}

// Checks that assigning a value to a variable works properly
// ----------------------------------------------------------------------------
TEST_F (VartableFixture, AssigningVartable) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create an empty table of CSP variables, i.e., with no values at all
        vartable_t vartable;

        // and now populate it with up to NB_VARIABLES and their domains
        vector<string> names;
        vector<pair<int, int>> indices;
        populate (vartable, NB_VARIABLES, NB_VALUES, names, indices);

        // randomly pick up up to NB_VARIABLES random numbers which will stand
        // for the value assigned to each variable
        vector<int> value = randVectorInt (NB_VARIABLES, NB_VALUES);

        // set these values to the CSP variables
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {
            vartable.assign (j, value[j]);
        }

        // Now, traverse the whole table of CSP variables, verifying that the
        // value assigned to each variable is the expected one
        for (auto j = 0 ; j < NB_VARIABLES ; j++) {

            ASSERT_EQ (vartable.get_value(j), value[j]);
        }
    }
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
