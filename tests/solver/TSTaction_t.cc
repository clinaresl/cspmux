// -*- coding: utf-8 -*-
// TSTaction_t.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-08-2021 15:18:17.890252376 (1628860697)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests for testing MUX solver actions

#include "../TSThelpers.h"
#include "../fixtures/TSTactionfixture.h"

// Checks that actions can be properly invoked
// ----------------------------------------------------------------------------
vector<int> actionContainer;

TEST_F (ActionFixture, EmptyManager) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an action with a lambda function which adds two integers to a
        // container of ints
        auto ints = randVectorInt (2, NB_VALUES);

        // create the action with a lambda function which adds its arguments to
        // the container. Note that the value of the previous and new element
        // are needless in this unit test and that the index is not used at all
        action_t action { [] (size_t index, int val1, int val2) {
            actionContainer.push_back (val1);
            actionContainer.push_back (val2);
        }, 0, ints[0], ints[1]};

        // execute the action over an empty vector
        actionContainer.clear ();
        action.exec();

        // and verify the container has precisely the two elements that have
        // been randomly chosen
        ASSERT_EQ (actionContainer.size (), 2);
        ASSERT_TRUE (find (actionContainer.begin (), actionContainer.end (), ints[0]) != actionContainer.end () &&
                     find (actionContainer.begin (), actionContainer.end (), ints[1]) != actionContainer.end ());
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
