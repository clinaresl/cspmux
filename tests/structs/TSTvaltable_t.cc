// -*- coding: utf-8 -*-
// TSTvaltable_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 21:26:31.191145832 (1628537191)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of tables of MUX values

#include "../TSThelpers.h"
#include "../fixtures/TSTvaltablefixture.h"

// Checks the creation of a table of values generates an empty table. By
// default, the type of values is int
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, EmptyValtable) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty table of values, i.e., with no values at all
        valtable_t<int> valtable;

        // and verify it consists of precisely 0 elements
        ASSERT_EQ (valtable.size (), 0);
    }
}

// Checks that updating the number of values of each entry of the table of
// values correctly works
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, NbValuesValtable) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values, i.e., with no values at all
        valtable_t<int> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorInt (rand () % MAX_LENGTH/1000, MAX_LENGTH/1000);

        // insert all items in the table and verify that the value returned is
        // the last index
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.insert(items[j]), j);
        }

        // In passing verify that the size of the table of values is the
        // expected one
        ASSERT_EQ (valtable.size (), items.size ());

        // now, randomly choose the number of values of each value of the table.
        // Note that values have to be written prior to set their number of
        // values
        vector<int> nbvalues = randVectorInt (valtable.size (), MAX_LENGTH/1000);

        // and set all these numbers of values
        for (auto j = 0 ; j < valtable.size () ; j++) {
            valtable.set_nbvalues(j, nbvalues[j]);
        }

        // and finally check that these numbers have been properly stored
        for (auto j = 0 ; j < items.size (); j++) {
            ASSERT_EQ (valtable.get_nbvalues (j), nbvalues[j]);
        }
    }
}

// Checks the insertion and extraction of tables of int values
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, IntValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values
        valtable_t<int> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorInt (rand () % MAX_LENGTH/1000, MAX_LENGTH/1000);

        // insert all items in the table and verify that the value returned is
        // the last index
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.insert(items[j]), j);
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.get_value (j).get_value (), items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Checks the insertion and extraction of tables of string values
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, StringValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values
        valtable_t<string> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorString (rand () % MAX_LENGTH/1000, 10);

        // insert all items in the table and verify that the value returned is
        // the last index
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.insert(items[j]), j);
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.get_value (j).get_value (), items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Checks the insertion and extraction of tables of time values ---which are
// essentially ints 
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, TimeValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values
        valtable_t<time_t> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorTime (rand () % MAX_LENGTH/1000,
                                     time (nullptr));

        // insert all items in the table and verify that the value returned is
        // the last index
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.insert(items[j]), j);
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable.get_value (j).get_value (), items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
