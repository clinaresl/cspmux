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

#include "../TSThelpers.h"
#include "../fixtures/TSTbmapfixture.h"

// Checks the creation of empty bitmaps is possible --but useless
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, EmptyBitmaps) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty bitmap, ie., with 0 bits
        bmap_t bmap (0);

        // and now test the size is precisely 0 bits
        ASSERT_EQ (bmap.size (), 0);
    }
}

// Verifies that bitmaps are created with the requested length when it is
// divisible by 8
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, ExactSize) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly determine the size of the bitmap. Note that the max size is
        // divided by 8 ...
        size_t bsize = rand () % MAX_LENGTH/8;

        // create a bitmap with the size multiplyin gnow by eight so that it is
        // guaranteed to have a size which is a multiple of 8
        bmap_t bmap (8*bsize);

        // and now verify that it has the desired size
        ASSERT_EQ (8*bsize, bmap.size ());
    }
}

// Verifies that bitmaps are created with the requested length when it is
// divisible by 8
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, InexactSize) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // randomly generate a size which is not divisible by 8
        size_t bsize;
        do {

            // randomly choose the size of this bitmap
            bsize = rand () % MAX_LENGTH;
        } while (bsize%8 ==0);

        // create a bitmap with the specified size
        bmap_t bmap (bsize);

        // and now verify that it takes one additional byte
        ASSERT_EQ (8*(bsize/8 + 1), bmap.size ());
    }
}

// Check that bitmaps are null upon creation
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, NullBitmap) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++ ) {

        // randomly generate the size of the bitmap. Note that because all
        // positions are going to be traversed, the size of the bitmap is much
        // less than the maximum size
        bmap_t bmap(rand () % MAX_LENGTH/1000);

        // check all positions to verify that they are all null
        for (auto j = 0 ; j < bmap.size (); j++) {

            // using both get and operator[]
            ASSERT_EQ (bmap.get (j), false);
            ASSERT_EQ (bmap[j], false);
        }
    }
}

// Check that bitmaps can set all locations
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, FullBitmap) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++ ) {

        // randomly generate the size of the bitmap. Note that because all
        // positions are going to be traversed, the size of the bitmap is much
        // less than the maximum size
        bmap_t bmap(rand () % MAX_LENGTH/1000);

        // first, set all bits of the bitmap
        for (auto j = 0 ; j < bmap.size (); j++) {
            bmap.set (j, true);
        }

        // second, check all positions to verify that they are all set
        for (auto j = 0 ; j < bmap.size (); j++) {

            // using both get and operator[]
            ASSERT_EQ (bmap.get (j), true);
            ASSERT_EQ (bmap[j], true);
        }
    }
}

// Check that individual bits can be set and retrieved correctly
// ----------------------------------------------------------------------------
TEST_F (BitmapFixture, PartialBitmap) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++ ) {

        // randomly generate the size of the bitmap. Note that because all
        // positions are going to be traversed, the size of the bitmap is much
        // less than the maximum size
        size_t bsize = rand () % MAX_LENGTH/1000;
        bmap_t bmap(bsize);

        // randomly determine the bits to set
        auto setbits = randSetInt (bsize/10, bsize);

        // first, set all locations randomly chosen
        for (auto it : setbits) {
            bmap.set (it, true);
        }

        // second, check all positions to verify that they are correctly
        // retrieved
        for (auto j = 0 ; j < bmap.size (); j++) {

            // point is, the j-th location is either false or true. If it is
            // false, then this location should not be found among the bits
            // randomly chosen; otherwise, it should be found
            ASSERT_TRUE ((bmap[j] && setbits.find (j) != setbits.end ()) ||
                         (!bmap[j] && setbits.find (j) == setbits.end ()));
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
