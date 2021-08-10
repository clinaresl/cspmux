// -*- coding: utf-8 -*-
// TSTvariable_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mar 10-08-2021 00:08:59.648110934 (1628546939)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

#include "../TSThelpers.h"
#include "../fixtures/TSTvariablefixture.h"

// Checks the creation of variables without a description. It verifies the name
// is correctly stored and that the description is empty
// ----------------------------------------------------------------------------
TEST_F (VariableFixture, EmptyDescription) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a new variable with a random name and no description
        string name = randString (20);
        variable_t variable (name);

        // verify the name has been correctly stored
        ASSERT_EQ (variable.get_name (), name);

        // and also that the description is empty
        ASSERT_EQ (variable.get_description (), "");
    }
}

// Checks the creation of variables with a non-null description. It verifies
// that both the name and the description are correctly stored
// ----------------------------------------------------------------------------
TEST_F (VariableFixture, NonEmptyDescription) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a new variable with a random name and description
        string name = randString (20);
        string description = randString (100);
        variable_t variable (name, description);

        // verify both the name and the description have been correctly stored
        ASSERT_EQ (variable.get_name (), name);
        ASSERT_EQ (variable.get_description (), description);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
