// -*- coding: utf-8 -*-
// TSTframe_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 17:49:30.777797687 (1628869770)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Defines a Google test fixture for testing MUX frames

#include<algorithm>

#include "../TSThelpers.h"
#include "../fixtures/TSTframefixture.h"

// Verify that actions can be pushed to a frame
// ----------------------------------------------------------------------------
TEST_F (FrameFixture, PushFrame) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty frame
        frame_t frame;

        // verify its size is strictly equal to 0
        ASSERT_EQ (frame.size (), 0);

        // create a random number of actions
        int nbactions = 1 + rand () % NB_VALUES;

        for (auto j = 0 ; j < nbactions ; j++) {

            // all actions have the same lambda function which actually does
            // nothing and receives a null index and two random numbers
            frame.push (action_t {[] (size_t index, size_t val1, size_t val2) {},
                0, size_t(rand ()), size_t(rand ())});
        }

        // now, verify there are exactly as many actions in this frame as
        // expected
        ASSERT_EQ (frame.size (), nbactions);
    }
}

vector<int> frameContainer;

// Verify that actions of a frame are correctly executed
// ----------------------------------------------------------------------------
TEST_F (FrameFixture, ExecFrame) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty frame
        frame_t frame;

        // create a random number of actions, each for inserting two numbers
        // into a vector
        int nbactions = 1+rand () % NB_VALUES;

        // randomly determine the arguments to use in every action
        vector<int> ints = randVectorInt (2*nbactions, NB_VALUES);

        // and add actions to the frame
        for (auto j = 0 ; j < nbactions ; j++) {

            // all actions have the same lambda function which inserts two
            // numbers (in reversed order) in a vector and receives the
            // arguments to insert ---note the index is uselss in this tests
            frame.push (action_t {[] (size_t index, size_t val1, size_t val2) {
                frameContainer.push_back (val2);
                frameContainer.push_back (val1);
            }, 0, size_t(ints[j*2]), size_t(ints[j*2+1])});
        }

        // in passing verify (again!) there are exactly as many actions in this
        // frame as expected
        ASSERT_EQ (frame.size (), nbactions);

        // clear the container and execute the frame
        frameContainer.clear ();
        frame.exec();

        // verify now the container is strictly equal to the vector of random
        // ints in reversed order. The reason why this is expected is: first,
        // the lambda function reverses every pair; second, actions in a frame
        // are executed in reversed order (and we are here implicitly checking
        // this also!)
        reverse(ints.begin (), ints.end ());
        ASSERT_EQ (ints, frameContainer);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
