// -*- coding: utf-8 -*-
// TSTmanagerfixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 11-08-2021 12:56:58.685229649 (1628679418)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of managers

#ifndef _TSTMANAGERFIXTURE_H_
#define _TSTMANAGERFIXTURE_H_

#include<cstdlib>
#include<ctime>
#include<utility>

#include <iostream>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/solver/MUXmanager.h"

// Class definition
//
// Defines a Google test fixture for testing MUX tables of variables
class ManagerFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }

        // return a randomly generated vector of strings and integer values to
        // populate the domain of n variables named after the strings randomly
        // selected. For each variable, the number of values generated is
        // randomly selected in the interval [10, NB_VALUES] and the integer
        // values are randomly selected in the interval [0, MAX_LENGTH]
        void randVarIntVals (int n, vector<string>& name, vector<vector<value_t<int>>>& values) {

            // first, randomly select a name for a variable
            name = randVectorString (n, 10);

            // and now randomly select integer values for the domain of each
            // variable ensuring that no values are repeated in the same domain
            for (int i = 0 ; i < n ; i++) {
                vector<int> raw_values = randVectorInt (1 + (rand () % NB_VALUES),
                                                        MAX_LENGTH/1000,
                                                        true);

                // and now transform these values into MUX values
                vector<value_t<int>> ivalues;
                for (auto value : raw_values) {
                    ivalues.push_back (value_t<int>{value});
                }

                // and add the domain of the i-th variable
                values.push_back (ivalues);
            }
        }

        // return a randomly generated vector of strings and strings to populate
        // the domain of each variable. The number of values generated is also
        // randomly selected in the interval [0, MAX_LENGTH/1000)
        void randVarStringVals (int n, vector<string>& name, vector<vector<value_t<string>>>& values) {

            // first, randomly select a name for a variable
            name = randVectorString (n, 10);

            // and now randomly select integer values for the domain of each
            // variable ensuring that no values are repeated in the same domain
            for (int i = 0 ; i < n ; i++) {
                vector<string> raw_values = randVectorString (1+rand ()% NB_VALUES, 20, true);

                // and now transform these values into MUX values
                vector<value_t<string>> ivalues;
                for (auto value : raw_values) {
                    ivalues.push_back (value_t<string>(value));
                }

                // and add the domain of the i-th variable
                values.push_back (ivalues);
            }
        }

        // return a randomly generated vector of strings and times to populate
        // the domain of each variable. The number of values generated is also
        // randomly selected in the interval [0, MAX_LENGTH/1000)
        void randVarTimeVals (int n, vector<string>& name, vector<vector<value_t<time_t>>>& values) {

            // first, randomly select a name for a variable
            name = randVectorString (n, 10);

            // and now randomly select integer values for the domain of each
            // variable
            for (int i = 0 ; i < n ; i++) {
                vector<time_t> raw_values = randVectorTime (1+rand ()% NB_VALUES,
                                                            time (nullptr),
                                                            true);

                // and now transform these values into MUX values
                vector<value_t<time_t>> ivalues;
                for (auto value : raw_values) {
                    ivalues.push_back (value_t<time_t>(value));
                }

                // and add the domain of the i-th variable
                values.push_back (ivalues);
            }
        }

        // add all variables to the specified manager named after the vector of
        // strings with domains given in the matrix of values specified
        template <typename T>
        void addVariables (manager<T>& m,
                           vector<string>& names, vector<vector<value_t<T>>>& values) {

            // create variables with the information provided
            for (int j = 0 ; j < names.size () ; j++) {

                // add the i-th variable along with its domain
                variable_t newvar = variable_t {names[j]};
                m.add_variable (newvar, values[j]);
            }
        }

        // given two locations of a matrix of instances of value_t return a
        // vector of pairs with the cross product of all values
        template<typename T>
        vector<pair<T, T>> cross_product (int idx1, int idx2,
                                          vector<vector<value_t<T>>>& values) {

            // initialize the output
            vector<pair<T, T>> output;

            // compute the cross product
            for (auto val1 : values[idx1]) {
                for (auto val2 : values[idx2]) {
                    output.push_back (pair<T, T> {val1.get_value (),
                        val2.get_value ()});
                }
            }

            // and return the cross product
            return output;
        }

        // return a pair with the first and last index to the values in the
        // domain of the i-th variable given a matrix of values
        template<typename T>
        pair<size_t, size_t> get_bounds (size_t i, vector<vector<value_t<T>>>& values) {

            size_t first = 0;

            // add the number of items in the domain of each variable preceding
            // i by the size of the domain
            for (auto idx = 0 ; idx < i ; first += values[idx++].size ());

            // add the lenght of this domain to get the index to the last value,
            // and return the boundaries
            return pair<size_t, size_t> {first, first + values[i].size ()-1};
        }
};

#endif // _TSTMANAGERFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
