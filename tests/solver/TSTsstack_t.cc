// -*- coding: utf-8 -*-
// TSTsstack_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 17:38:57.368832494 (1628869137)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests for testing MUX solver stacks

#include<algorithm>
#include<numeric>

#include "../TSThelpers.h"
#include "../fixtures/TSTsstackfixture.h"

// Checks that frames are effectively pushed onto a stack
// ----------------------------------------------------------------------------
TEST_F (SstackFixture, PushSstack) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, create an empty stack
        sstack_t stack;

        // verify its size is strictly equal to 0
        ASSERT_EQ (stack.size (), 0);

        // and now randomly decide the number of frames to insert
        int nbframes = 1 + rand () % NB_VALUES;

        for (auto j = 0 ; j < nbframes ; j++) {

            // create an empty frame
            frame_t frame;

            // create a random number of actions
            int nbactions = 1 + rand () % NB_VALUES;

            // and insert a number of dummy actions
            for (auto k = 0 ; k < nbactions ; k++) {

                // all actions have the same lambda function which actually does
                // nothing and receives a null index and two random numbers
                frame.push (action_t {[] (size_t index, int val1, int val2) {},
                    0, rand (), rand ()});
            }

            // in passing, why not, verify that the size of the frame is the
            // expected one
            ASSERT_EQ (frame.size (), nbactions);

            // push this frame into the stack
            stack.push (frame);

            // and verify the size of the stack
            ASSERT_EQ (stack.size (), 1+j);
        }

        // before leaving, ensure the stack has as many frames as the number
        // randomly selected (anyway, that was already verified in the last
        // iteration of the preceding loop)
        ASSERT_EQ (stack.size (), nbframes);
    }
}

int sum;
vector<int> stackContainer;

// Checks that stacks are effectively unwound step by step
// ----------------------------------------------------------------------------
TEST_F (SstackFixture, UnwindSstack) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, create an empty stack
        sstack_t stack;

        // verify its size is strictly equal to 0
        ASSERT_EQ (stack.size (), 0);

        // ---First frame

        // create an empty frame
        frame_t frame0;

        // create a random number of actions, each for adding two numbers
        int nbactions = 1+rand () % NB_VALUES;

        // randomly determine the arguments to use in every action
        vector<int> ints = randVectorInt (2*nbactions, NB_VALUES);

        // and add actions to the frame
        for (auto j = 0 ; j < nbactions ; j++) {

            // all actions have the same lambda function which adds two numbers
            // and receives the arguments to add ---note the index is uselss in
            // this tests
            frame0.push (action_t {[] (size_t index, int val1, int val2) {
                sum += (val1 + val2);
            }, 0, ints[j*2], ints[j*2+1]});
        }

        // push this frame onto the stack
        stack.push (frame0);

        // ---Second frame

        // create an empty frame
        frame_t frame1;

        // and add actions to the frame
        for (auto j = 0 ; j < nbactions ; j++) {

            // all actions have the same lambda function which inserts two
            // numbers (in reversed order) in a vector and receives the
            // arguments to insert ---note the index is uselss in this tests
            frame1.push (action_t {[] (size_t index, int val1, int val2) {
                stackContainer.push_back (val2);
                stackContainer.push_back (val1);
            }, 0, ints[j*2], ints[j*2+1]});
        }

        // push this frame onto the stack
        stack.push (frame1);

        // unwind the second frame (the one at the top) after clearing the
        // output container
        stackContainer.clear ();
        stack.unwind();

        // and verify that the vector has been effectively reversed
        reverse (ints.begin (), ints.end ());
        ASSERT_EQ (ints, stackContainer);

        // unwind the first frame (the one at the bottom) after initializing the
        // output accumulation parameter
        sum = 0;
        stack.unwind ();

        // and verify the result is the expected one
        auto rsum = accumulate(ints.begin(), ints.end(), 0);
        ASSERT_EQ (sum, rsum);
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
