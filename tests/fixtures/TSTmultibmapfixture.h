// -*- coding: utf-8 -*-
// TSTmultibmapfixture.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 01:01:43.491136093 (1628290903)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Test fixture for testing CSPMUX multibitmaps

#ifndef _TSTMULTIBMAPFIXTURE_H_
#define _TSTMULTIBMAPFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/structs/MUXmultibmap_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX multibitmaps
class MultibitmapFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTMULTIBMAPFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
