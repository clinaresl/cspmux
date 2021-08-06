// -*- coding: utf-8 -*-
// TSTbmapfixture.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 01:01:43.491136093 (1628290903)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Test fixture for testing CSPMUX bitmaps

#ifndef _TSTBMAPFIXTURE_H_
#define _TSTBMAPFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/MUXbmap_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX bitmaps
class BitmapFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTBMAPFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
