// -*- coding: utf-8 -*-
// TSTvariable_t.h
// -----------------------------------------------------------------------------
//
// Started on <mar 10-08-2021 00:07:19.061999940 (1628546839)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

#ifndef _TSTVARIABLE_T_H_
#define _TSTVARIABLE_T_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../src/structs/MUXvariable_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX bitmaps
class VariableFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};



#endif // _TSTVARIABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
