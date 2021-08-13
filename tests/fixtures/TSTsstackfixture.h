// -*- coding: utf-8 -*-
// TSTsstackfixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 17:34:15.986461233 (1628868855)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests for testing MUX solver stacks

#ifndef _TSTSSTACKFIXTURE_H_
#define _TSTSSTACKFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/solver/MUXsstack_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX bitmaps
class SstackFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};


#endif // _TSTSSTACKFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
