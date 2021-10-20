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

// Check adding and reading int values from a valtable
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, AddIntValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<int> valtable;
        auto items = randVectorInt (rand () % MAX_LENGTH/1000, MAX_LENGTH/1000);

        // and now write all these values and verify whether the location
        // taken in the val table is correct or not
        for (auto j = 0 ; j < items.size () ; j++) {
            valtable += items[j];
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable[j], items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Check adding and reading string values from a valtable
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, AddStringValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<string> valtable;
        auto items = randVectorString (rand () % MAX_LENGTH/1000, 10);

        // and now write all these values and verify whether the location
        // taken in the val table is correct or not
        for (auto j = 0 ; j < items.size () ; j++) {
            valtable += items[j];
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable[j], items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Check adding and reading time values from a valtable
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, AddTimeValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<time_t> valtable;
        auto items = randVectorTime (rand () % MAX_LENGTH/1000, time (nullptr));

        // and now write all these values and verify whether the location
        // taken in the val table is correct or not
        for (auto j = 0 ; j < items.size () ; j++) {
            valtable += items[j];
        }

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable[j], items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Checks the insertion and extraction of tables of int values
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, InsertIntValues) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<int> valtable;
        vector<int> items;
        populate (valtable, rand ()% MAX_LENGTH/1000, items);

        // now, retrieve all values from the table of values and verify they've
        // are correct
        for (auto j = 0 ; j < items.size () ; j++) {

            ASSERT_EQ (valtable[j], items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Checks the insertion and extraction of tables of string values
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, InsertStringValues) {

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

            ASSERT_EQ (valtable[j], items[j]);
        }

        // Last check before leaving, make sure the size of the table is correct
        // as well
        ASSERT_EQ (valtable.size (), items.size ());
    }
}

// Checks the insertion and extraction of tables of time values ---which are
// essentially ints 
// ----------------------------------------------------------------------------
TEST_F(ValtableFixture, InsertTimeValues) {

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

// Checks that updating the number of values of each entry of the table of
// values correctly works
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, SetNbValuesValtable) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<int> valtable;
        vector<int> items;
        populate (valtable, rand ()% MAX_LENGTH/1000, items);

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

// Checks that decrementing the number of values of each entry of the table of
// values correctly works
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, DecrementNbValuesValtable) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<int> valtable;
        vector<int> items;
        populate (valtable, rand ()% MAX_LENGTH/1000, items);

        // In passing verify that the size of the table of values is the
        // expected one
        ASSERT_EQ (valtable.size (), items.size ());

        // now, randomly choose the number of values of each value of the table.
        // Note that values have to be written prior to set their number of
        // values
        vector<int> nbvalues = randVectorInt (valtable.size (), MAX_LENGTH/1000);

        // and set all these numbers of values
        for (auto j = 0 ; j < valtable.size () ; j++) {

            // the number of values shall be strictly positive, so that when
            // decrementing it an "out of bounds" exception is not thrown
            valtable.set_nbvalues(j, 1+nbvalues[j]);
        }

        // and decrement them all
        for (auto j = 0 ; j < valtable.size () ; j++) {
            valtable.decrement_nbvalues(j);
        }

        // and check them all in a row
        for (auto j = 0 ; j < items.size (); j++) {

            // because we inserted one value above the number randomly chosen
            // for each position and then we decremented, we should now get the
            // same values originally chosen
            ASSERT_EQ (valtable.get_nbvalues (j), nbvalues[j]);
        }
    }
}

// Checks that incrementing the number of values of each entry of the table of
// values correctly works
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, IncrementNbValuesValtable) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly populate a val table
        valtable_t<int> valtable;
        vector<int> items;
        populate (valtable, rand ()% MAX_LENGTH/1000, items);

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

        // and increment them all
        for (auto j = 0 ; j < valtable.size () ; j++) {
            auto new_value = valtable.increment_nbvalues(j);
        }

        // and check them all in a row
        for (auto j = 0 ; j < items.size (); j++) {
            ASSERT_EQ (valtable.get_nbvalues (j), nbvalues[j]+1);
        }
    }
}

// Checks all vectors are enabled by default
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, MultivectorDefaultEnabled) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values, i.e., with no values at all
        valtable_t<int> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorInt (rand () % MAX_LENGTH/1000, MAX_LENGTH/1000);

        // insert all items in the table and verify that the value returned is
        // the last index. Note that values have to be inserted before updating
        // the number of values
        for (auto j = 0 ; j < items.size () ; j++) {

            // inserting an item returns the index where it is inserted
            ASSERT_EQ (valtable.insert(items[j]), j);

            // verify now that all new entries are enabled by default
            ASSERT_TRUE (valtable.get_status (j));
        }
    }
}

// Checks the status of all vectors can be independently set and retrieved
// ----------------------------------------------------------------------------
TEST_F (ValtableFixture, MultivectorSetStatus) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create an empty table of values, i.e., with no values at all
        valtable_t<int> valtable;

        // now, randomly generate a random number of values
        auto items = randVectorInt (rand () % MAX_LENGTH/1000, MAX_LENGTH/1000);

        // randomly decide the status of each entry of the multivector
        vector<int> status = randVectorInt (items.size (), 2);

        // insert all items in the table and verify that the value returned is
        // the last index. Note that values have to be inserted before updating
        // the number of values
        for (auto j = 0 ; j < items.size () ; j++) {

            // inserting an item returns the index where it is inserted
            ASSERT_EQ (valtable.insert(items[j]), j);

            // set the status of this entry. Another test already verified that
            // the status is enabled by default, so we intentionally update only
            // the status of disabled entries
            if (!status[j]) {
                valtable.set_status (j, false);
            }

            // verify the status has been correctly updated
            ASSERT_EQ (valtable.get_status (j), status[j]);
        }

        // make sure the table has as many entires as expected
        ASSERT_EQ(valtable.size (), items.size ());

        // next, go over the entire value table to verify the status of all
        // entries
        for (auto i = 0 ; i < valtable.size (); i++) {
            ASSERT_EQ (valtable.get_status (i), status[i]);
        }
    }
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
