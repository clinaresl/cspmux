// -*- coding: utf-8 -*-
// TSTvartable_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 21:27:54.184609161 (1628537274)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of tables of CSP variables

#ifndef _TSTVARTABLE_T_H_
#define _TSTVARTABLE_T_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/MUXvartable_t.h"

// Class definition
//
// Defines a Google test fixture for testing MUX tables of variables
class VartableFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }

        // populate the given var table with n different CSP variables, each
        // with a domain defined over m values. It also returns a couple of
        // additional vectors:
        //    names[j] is the name of the j-th CSP variable
        //    indices[j]: <first, last> indices to the values of its domain
        void populate (vartable_t& vartable,
                       int n, int m,
                       std::vector<std::string>& names, std::vector<std::pair<int,int>>& indices) {

            // create a vector to hold all variables that will be inserted into
            // this table, and randomly choose a name for each one
            std::vector<variable_t> variables;
            for (auto j = 0 ; j < n ; j++) {
                names.push_back (randString (50));
                variables.push_back (variable_t (names[j]));
            }

            // randomly simulate the domain of each variable by picking up two
            // indices, the second being larger than the first. Generate as many
            // pairs as variables
            indices = randVectorIntPair (n, MAX_LENGTH/1000, 1000);

            // and now add these variables along with their domain to the table of
            // CSP variables
            for (auto j = 0 ; j < n ; j++) {
                vartable.insert (variables[j],
                                 indices[j].first, indices[j].second);
            }
        }
};

#endif // _TSTVARTABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
