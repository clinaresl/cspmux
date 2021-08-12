// -*- coding: utf-8 -*-
// TSTmultivectorfixture.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-08-2021 12:49:48.804311213 (1628765388)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests OF CSPMUX multivectors

#ifndef _TSTMULTIVECTORFIXTURE_H_
#define _TSTMULTIVECTORFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/structs/MUXmultivector_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX multivectors
class MultivectorFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTMULTIVECTORFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
