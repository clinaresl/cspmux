// -*- coding: utf-8 -*-
// TSTbmap_t.cc
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 00:51:13.621574651 (1628290273)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of CSPMUX bitmaps

#include "../fixtures/TSTbmapfixture.h"

// Checks the creation of empty bitmaps is possible --but useless
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, EmptyBitmaps) {

    for (auto i = 0 ; i < NBTESTS ; i++) {

        // create an empty bitmap, ie., with 0 bits
        bmap_t bmap (0);

        // and now test the size is precisely 0 bits
        ASSERT_EQ (bmap.size (), 0);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
