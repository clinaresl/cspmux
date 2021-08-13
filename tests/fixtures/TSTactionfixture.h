// -*- coding: utf-8 -*-
// TSTactionfixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 15:17:25.789066957 (1628860645)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests for testing MUX solver actions

#ifndef _TSTACTIONFIXTURE_H_
#define _TSTACTIONFIXTURE_H_

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/solver/MUXaction_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX actions
class ActionFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};


#endif // _TSTACTIONFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
