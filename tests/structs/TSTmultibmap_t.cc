// -*- coding: utf-8 -*-
// TSTmultibmap_t.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 08-08-2021 15:42:45.681964645 (1628430165)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Unit tests of CSPMUXX multibitmaps

#include "../TSThelpers.h"
#include "../fixtures/TSTmultibmapfixture.h"

// Checks the creation of empty multibitmaps is possible ---but useless
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, EmptyMultibitmaps) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty bitmap, ie., with 0 bits
        multibmap_t multibmap (0, 0);

        // and now test the size is precisely 0 bits
        ASSERT_EQ (multibmap.size (), 0);
    }
}

// Checks the creation of non-empty multibitmaps with null bitmaps is possible
// ---but useless
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, MultibitmapsEmptyBitmaps) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a bitmap with a random length
        size_t mbsize = random () % MAX_LENGTH/1000000;
        multibmap_t multibmap (mbsize, 0);

        // check now the multibitmap has precisely that number of entries
        ASSERT_EQ (multibmap.size (), mbsize);

        // and now check that each entry consists of a null bitmap
        for (auto j = 0 ; j < mbsize ; j++ ) {
            ASSERT_EQ (multibmap[j].size (), 0);
        }
    }
}

// Checks the creation of non-empty multibitmaps with non-null bitmaps
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, MultibitmapsNonEmptyBitmaps) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a bitmap with a random length where each bitmap has also a
        // random length
        size_t mbsize = random () % MAX_LENGTH/1000000;
        size_t bsize = random () % MAX_LENGTH/1000000;
        multibmap_t multibmap (mbsize, bsize);

        // check now the multibitmap has precisely that number of entries
        ASSERT_EQ (multibmap.size (), mbsize);

        // and now check that each entry consists of bitmaps with the specified
        // length

        // bitmaps have always a size which is divisible by 8, so compute in
        // advance the expected size of this bitmap
        bsize = 8*((bsize/8) + int(bsize%8!=0));
        for (auto j = 0 ; j < mbsize ; j++ ) {
            ASSERT_EQ (multibmap[j].size (), bsize);
        }
    }
}

// Checks all bitmaps are enabled by default
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, MultibitmapsEnabled) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a bitmap with a random length where each bitmap has also a
        // random length
        size_t mbsize = random () % MAX_LENGTH/1000000;
        size_t bsize = random () % MAX_LENGTH/1000000;
        multibmap_t multibmap (mbsize, bsize);

        // check that all entries are enabled by default
        for (auto j = 0 ; j < mbsize ; j++ ) {
            ASSERT_TRUE (multibmap.get_status (j));
        }
    }
}

// Checks that all bitmaps are null upon creation
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, MultibitmapsNullBitmaps) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create a bitmap with a random length where each bitmap has also a
        // random length
        size_t mbsize = random () % MAX_LENGTH/1000000;
        size_t bsize = random () % MAX_LENGTH/1000000;
        multibmap_t multibmap (mbsize, bsize);

        // check that all bits are false by default
        for (auto j = 0 ; j < mbsize ; j++ ) {
            for (auto k = 0 ; k < multibmap[0].size (); k++) {
                ASSERT_FALSE (multibmap[j][k]);
            }
        }
    }
}

// Checks that bitmaps can be read and written properly
// ----------------------------------------------------------------------------
TEST_F (MultibitmapFixture, MultibitmapsWriteRead) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create a bitmap with a random length where each bitmap has also a
        // random length
        size_t mbsize = random () % MAX_LENGTH/1000000;
        size_t bsize = random () % MAX_LENGTH/1000000;
        multibmap_t multibmap (mbsize, bsize);

        // get the true size of the bitmaps
        bsize = multibmap[0].size ();

        // randomly pick up some locations of the whole multibitmap
        auto locs = randSetInt (mbsize*bsize/10,
                                mbsize*bsize);

        // set to true all the locations randomly chosen
        for (auto loc : locs) {
            multibmap.set (loc/bsize, loc%bsize, true);
        }

        // traverse the whole multibitmap
        for (auto j = 0 ; j < mbsize ; j++ ) {
            for (auto k = 0 ; k < bsize; k++) {

                // Now, either this location is false and it can not be found in
                // locs, or it is true and then it can be found in locs
                ASSERT_TRUE ( (!multibmap[j][k] && locs.find (j*bsize+k) == locs.end ()) ||
                              (multibmap[j][k] && locs.find (j*bsize+k) != locs.end ()) );
            }
        }
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
