// -*- coding: utf-8 -*-
// TSTmultivector.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-08-2021 12:51:19.486964387 (1628765479)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of CSPMUX multivectors

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../fixtures/TSTmultivectorfixture.h"

// Checks the creation of empty multivector is possible ---but useless
// ----------------------------------------------------------------------------
TEST_F (MultivectorFixture, EmptyMultivector) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty multivector, ie., with 0 entries
        multivector_t multivector (0);

        // and now test the size is precisely 0
        ASSERT_EQ (multivector.size (), 0);
    }
}

// Checks the creation of non-empty multivector with null vectors is possible
// ----------------------------------------------------------------------------
TEST_F (MultivectorFixture, MultivectorEmptyVectors) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a multivector with a random length
        size_t mvsize = random () % MAX_LENGTH/1000000;
        multivector_t multivector (mvsize);

        // check now the multivector has precisely that number of entries
        ASSERT_EQ (multivector.size (), mvsize);

        // and now check that each entry consists of a null vector
        for (auto j = 0 ; j < mvsize ; j++ ) {
            ASSERT_EQ (multivector[j].size (), 0);
        }
    }
}

// Checks all vectors are enabled by default
// ----------------------------------------------------------------------------
TEST_F (MultivectorFixture, MultivectorEnabled) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a multivector with a random length
        size_t mvsize = random () % MAX_LENGTH/1000000;
        multivector_t multivector (mvsize);

        // check that all entries are enabled by default
        for (auto j = 0 ; j < mvsize ; j++ ) {
            ASSERT_TRUE (multivector.get_status (j));
        }
    }
}

// Checks the status of all vectors can be independently set and retrieved
// ----------------------------------------------------------------------------
TEST_F (MultivectorFixture, MultivectorStatus) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a multivector with a random length
        size_t mvsize = random () % MAX_LENGTH/1000000;
        multivector_t multivector (mvsize);

        // randomly decide the status of each entry of the multivector
        vector<int> status = randVectorInt (mvsize, 2);

        // and set the values in the multivector
        for (auto j = 0 ; j < mvsize ; j++) {
            multivector.set_status (j, status[j]);
        }

        // check that all entries have been properly set
        for (auto j = 0 ; j < mvsize ; j++ ) {
            ASSERT_EQ (multivector.get_status (j), status[j]);
        }
    }
}

// Checks that multivectors can be read and written properly
// ----------------------------------------------------------------------------
TEST_F (MultivectorFixture, MultivectorWriteRead) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create a multivector with a random length
        // create a multivector with a random length
        size_t mvsize = random () % MAX_LENGTH/1000000;
        multivector_t multivector (mvsize);

        // randomly pick up some locations of the whole multivector
        auto locs = randSetInt (mvsize*mvsize/50,
                                mvsize*mvsize);

        // set to true all the locations randomly chosen ---those addes are
        // meant to be true; whereas those absent are meant to be false
        for (auto loc : locs) {
            multivector.set (loc/mvsize, loc%mvsize);
        }

        // verify that all values have been properly set
        for (auto j = 0 ; j < mvsize ; j++) {
            for (auto k = 0 ; k < mvsize ; k++) {

                // Now, either this location is enabled (it is explicitly listed
                // in the multivector) or it is false, in which case it can not
                // be found in the multivector
                ASSERT_TRUE ( (!multivector.find (j, k) && locs.find (j*mvsize + k) == locs.end ()) ||
                              (multivector.find (j, k) && locs.find (j*mvsize + k) != locs.end ()) );
            }
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
