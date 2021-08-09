// -*- coding: utf-8 -*-
// TSTvaltable_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 21:27:54.184609161 (1628537274)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

#ifndef _TSTVALTABLE_T_H_
#define _TSTVALTABLE_T_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
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
};

#endif // _TSTVALTABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
