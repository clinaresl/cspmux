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

using namespace std;

// Checks that actions can be properly invoked
// ----------------------------------------------------------------------------
vector<int> actionAddContainer;

TEST_F (ActionFixture, AddActions) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an action with a lambda function which adds two integers to a
        // container of ints
        auto ints = randVectorInt (2, NB_VALUES);

        // create the action with a lambda function which adds its arguments to
        // the container. Note that in this case, the first parameter is useless
        action_t action { [] (size_t index, size_t val1, size_t val2) {
            actionAddContainer.push_back (val1);
            actionAddContainer.push_back (val2);
        }, 0, size_t(ints[0]), size_t(ints[1])};

        // execute the action over an empty vector
        actionAddContainer.clear ();
        action.exec();

        // and verify the container has precisely the two elements that have
        // been randomly chosen
        ASSERT_EQ (actionAddContainer.size (), 2);
        ASSERT_TRUE (find (actionAddContainer.begin (), actionAddContainer.end (), ints[0]) != actionAddContainer.end () &&
                     find (actionAddContainer.begin (), actionAddContainer.end (), ints[1]) != actionAddContainer.end ());
    }
}

vector<int> actionRemoveContainer;

TEST_F (ActionFixture, RemoveActions) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an action with a lambda function which adds two integers to a
        // container of ints
        auto ints = randVectorInt (2, NB_VALUES);

        // add these numbers to the container
        actionRemoveContainer.push_back (ints[0]);
        actionRemoveContainer.push_back (ints[1]);

        // verify the size of this container is precisely two
        ASSERT_EQ (actionRemoveContainer.size (), 2);

        // create the action with a lambda function which removes the last item
        // of the container. Note that in this case, the parameters are useless
        action_t action { [] (size_t index, size_t val1, size_t val2) {
            actionRemoveContainer.pop_back ();
            actionRemoveContainer.pop_back ();
        }, 0, 0, 0};

        // execute the action
        action.exec();

        // and verify the container is now empty
        ASSERT_EQ (actionRemoveContainer.size (), 0);
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
