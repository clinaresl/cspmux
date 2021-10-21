// -*- coding: utf-8 -*-
// TSTvaltable_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 21:27:54.184609161 (1628537274)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of tables of MUX values

#ifndef _TSTVALTABLE_T_H_
#define _TSTVALTABLE_T_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/MUXvaltable_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX tables of values
class ValtableFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }

        // populate the given val table with n different values, each one
        // holding an integer value which are returned in values
        void populate (valtable_t<int>& valtable,
                       int n, std::vector<int>& values) {

            // randomly determine the int values to write in each location
            values = randVectorInt (n, MAX_LENGTH/1000);

            // and now write all these values and verify whether the location
            // taken in the val table is correct or not
            for (auto j = 0 ; j < values.size () ; j++) {
                ASSERT_EQ (valtable.insert (values[j]), j);
            }
        }
};

#endif // _TSTVALTABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
