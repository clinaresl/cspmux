// -*- coding: utf-8 -*-
// TSTframefixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 17:48:46.520191879 (1628869726)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

#ifndef _TSTFRAMEFIXTURE_H_
#define _TSTFRAMEFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/solver/MUXframe_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX frames
class FrameFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTFRAMEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
