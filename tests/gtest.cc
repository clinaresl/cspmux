// -*- coding: utf-8 -*-
// gtest.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 06-08-2021 23:56:53.449451914 (1628287013)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Main entry point of Google Tests

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Run specific tests only
    // testing::GTEST_FLAG(filter) = "VartableFixture.*";

    // Exclude specific tests
    // testing::GTEST_FLAG(filter) = "-BitmapFixture"; // The writing test is broken, so skip it

    // and run the selection of tests
    return RUN_ALL_TESTS();
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
