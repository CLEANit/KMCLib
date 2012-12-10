/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemap.h"

// Include the files to test.
#include "../src/latticemap.h"

#include "../src/coordinate.h"

#include <algorithm>

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testConstructionAndQuery()
{
    // Construct a map.

    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;

    std::vector<bool> periodicity(3, true);

    CPPUNIT_ASSERT_NO_THROW( LatticeMap map(3, repetitions, periodicity) );

    LatticeMap map(3, repetitions, periodicity);

    // Test the query functions.
    CPPUNIT_ASSERT( map.periodicA() );
    CPPUNIT_ASSERT( map.periodicB() );
    CPPUNIT_ASSERT( map.periodicC() );

    periodicity[0] = false;
    LatticeMap map2(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map2.periodicA() );
    CPPUNIT_ASSERT(  map2.periodicB() );
    CPPUNIT_ASSERT(  map2.periodicC() );

    periodicity[1] = false;
    LatticeMap map3(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map3.periodicA() );
    CPPUNIT_ASSERT( !map3.periodicB() );
    CPPUNIT_ASSERT(  map3.periodicC() );

    periodicity[2] = false;
    LatticeMap map4(3, repetitions, periodicity);
    CPPUNIT_ASSERT( !map4.periodicA() );
    CPPUNIT_ASSERT( !map4.periodicB() );
    CPPUNIT_ASSERT( !map4.periodicC() );

}

// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testIndicesFromCell()
{
    // LOCAL SCOPE FOR CLARITY
    {
        // A map with 1 in the basis.
        const int basis = 1;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );
                    ++index;
                }
            }
        }
    }
    {
        // A map with 1 in the basis and other periodicity.
        const int basis = 1;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, true);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[0], index );
                    ++index;
                }
            }
        }
    }
    {
        // A map with basis 2
        const int basis = 2;
        std::vector<int> repetitions(3);
        repetitions[0] = 3;
        repetitions[1] = 2;
        repetitions[2] = 4;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    for (int l = 0; l < basis; ++l) {
                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );
                        ++index;
                    }
                }
            }
        }
    }

    {
        // A huge map with basis 12 (1 200 000 points)
        const int basis = 12;
        std::vector<int> repetitions(3);
        repetitions[0] = 100;
        repetitions[1] = 100;
        repetitions[2] = 10;
        const std::vector<bool> periodicity(3, false);
        const LatticeMap map(basis, repetitions, periodicity);

        // Here indices from cell should just be the incrementat number.
        int index = 0;
        for (int i = 0; i < repetitions[0]; ++i) {
            for (int j = 0; j < repetitions[1]; ++j) {
                for (int k = 0; k < repetitions[2]; ++k) {
                    for (int l = 0; l < basis; ++l) {

                        CPPUNIT_ASSERT_EQUAL( map.indicesFromCell(i,j,k)[l], index );
                        ++index;
                    }
                }
            }
        }
    }

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndices()
{
    // Test that the neighbour indices are calculated and returned correctly.

    // Simplest case, base one, no periodicity and the queried cell in the
    // middle. This should return all indices.

    const int basis = 1;
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 3;
    repetitions[2] = 3;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central cell index.
    const int central_index = map.indicesFromCell(1,1,1)[0];

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 3*3*3);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 26
    std::sort(neighbours.begin(), neighbours.end());
    for (int i = 0; i < 3*3*3; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(neighbours[i], i);
    }

    // Now, check any other cell with periodicity.
    // The neighbours should be the same
    // after sorting.
    const std::vector<bool> periodicity2(3, true);
    const LatticeMap map2(basis, repetitions, periodicity2);

    std::vector<int> neighbours2 = map2.neighbourIndices(0);
    std::sort(neighbours2.begin(), neighbours2.end());

    std::vector<int> neighbours3 = map2.neighbourIndices(26);
    std::sort(neighbours3.begin(), neighbours3.end());

    // Check.
    for (size_t i = 0; i < neighbours.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(neighbours3[i], neighbours[i]);
    }

    // Remove periodicity from the first two directions.
    std::vector<bool> periodicity3(3, false);
    periodicity3[2] = true;
    const LatticeMap map3(basis, repetitions, periodicity3);

    // This should cut the 15 wrapped indices in the a and b directions.
    const std::vector<int> & neighbours4 = map3.neighbourIndices(0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours4.size()), static_cast<int>(neighbours.size())-15);

    // We should have indices: 2,0,1
    //                         5,3,4
    //                         11,9,10
    //                         14,12,13

    CPPUNIT_ASSERT_EQUAL(neighbours4[0], 2);
    CPPUNIT_ASSERT_EQUAL(neighbours4[1], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours4[2], 1);

    CPPUNIT_ASSERT_EQUAL(neighbours4[3], 5);
    CPPUNIT_ASSERT_EQUAL(neighbours4[4], 3);
    CPPUNIT_ASSERT_EQUAL(neighbours4[5], 4);

    CPPUNIT_ASSERT_EQUAL(neighbours4[6], 11);
    CPPUNIT_ASSERT_EQUAL(neighbours4[7], 9);
    CPPUNIT_ASSERT_EQUAL(neighbours4[8], 10);

    CPPUNIT_ASSERT_EQUAL(neighbours4[9], 14);
    CPPUNIT_ASSERT_EQUAL(neighbours4[10], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours4[11], 13);

    // Removing periodicity in the x,z directions instead we should have:
    // 6,7,0,1,3,4,15,16,9,10,12,13
    std::vector<bool> periodicity4(3, false);
    periodicity4[1] = true;
    const LatticeMap map4(basis, repetitions, periodicity4);

    const std::vector<int> & neighbours5 = map4.neighbourIndices(0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours5.size()), static_cast<int>(neighbours.size())-15);

    CPPUNIT_ASSERT_EQUAL(neighbours5[0], 6);
    CPPUNIT_ASSERT_EQUAL(neighbours5[1], 7);
    CPPUNIT_ASSERT_EQUAL(neighbours5[2], 0);

    CPPUNIT_ASSERT_EQUAL(neighbours5[3], 1);
    CPPUNIT_ASSERT_EQUAL(neighbours5[4], 3);
    CPPUNIT_ASSERT_EQUAL(neighbours5[5], 4);

    CPPUNIT_ASSERT_EQUAL(neighbours5[6], 15);
    CPPUNIT_ASSERT_EQUAL(neighbours5[7], 16);
    CPPUNIT_ASSERT_EQUAL(neighbours5[8],  9);

    CPPUNIT_ASSERT_EQUAL(neighbours5[9],  10);
    CPPUNIT_ASSERT_EQUAL(neighbours5[10], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours5[11], 13);

    // Test with basis 2.
    const LatticeMap map6(basis*2, repetitions, periodicity4);
    const std::vector<int> & neighbours6 = map6.neighbourIndices(0);

    // Expecting twice the size.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(neighbours6.size())/2, static_cast<int>(neighbours.size())-15);

    // Checking the first half of the indices.
    CPPUNIT_ASSERT_EQUAL(neighbours6[0], 12);
    CPPUNIT_ASSERT_EQUAL(neighbours6[1], 13);
    CPPUNIT_ASSERT_EQUAL(neighbours6[2], 14);

    CPPUNIT_ASSERT_EQUAL(neighbours6[3], 15);
    CPPUNIT_ASSERT_EQUAL(neighbours6[4],  0);
    CPPUNIT_ASSERT_EQUAL(neighbours6[5],  1);

    CPPUNIT_ASSERT_EQUAL(neighbours6[6],  2);
    CPPUNIT_ASSERT_EQUAL(neighbours6[7],  3);
    CPPUNIT_ASSERT_EQUAL(neighbours6[8],  6);

    CPPUNIT_ASSERT_EQUAL(neighbours6[9],  7);
    CPPUNIT_ASSERT_EQUAL(neighbours6[10], 8);
    CPPUNIT_ASSERT_EQUAL(neighbours6[11], 9);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndicesMinimal()
{
    // Minimal case, base two, no periodicity. One cell repetition.
    // This should return all indices in the cell.

    const int basis = 2;
    std::vector<int> repetitions(3);
    repetitions[0] = 1;
    repetitions[1] = 1;
    repetitions[2] = 1;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central index.
    const int central_index = 0;

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells times the basis.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 1*1*1*basis);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 1
    std::sort(neighbours.begin(), neighbours.end());
    CPPUNIT_ASSERT_EQUAL(neighbours[0], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours[1], 1);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testNeighbourIndicesMinimal2()
{
    // Minimal case, base two, no periodicity. One cell repetition.
    // This should return all indices in the cell.

    const int basis = 2;
    std::vector<int> repetitions(3);
    repetitions[0] = 1;
    repetitions[1] = 1;
    repetitions[2] = 2;
    const std::vector<bool> periodicity(3, false);
    const LatticeMap map(basis, repetitions, periodicity);

    // Get the central index.
    const int central_index = 1;

    // Get the neighbours.
    std::vector<int> neighbours = map.neighbourIndices(central_index);

    // The length of neighbours should be as many as there are cells times the basis.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(neighbours.size()), 1*1*2*basis);

    // If we sort the indices they should be just the incremental numbers
    // from 0 to 3
    std::sort(neighbours.begin(), neighbours.end());
    CPPUNIT_ASSERT_EQUAL(neighbours[0], 0);
    CPPUNIT_ASSERT_EQUAL(neighbours[1], 1);
    CPPUNIT_ASSERT_EQUAL(neighbours[2], 2);
    CPPUNIT_ASSERT_EQUAL(neighbours[3], 3);

    // DONE.
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testWrap()
{
    // Construct a periodic map.
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;
    const std::vector<bool> periodicity(3, true);
    LatticeMap map(3, repetitions, periodicity);

    // Check a few coordinates.

    {
        // Should be wrapped in all directions.
        Coordinate c(1.5, 2.5, 3.0);
        const Coordinate ref(-1.5, -2.5, -3.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should not be wrapped.
        Coordinate c(-1.5, -2.5, -3.0);
        const Coordinate ref(-1.5, -2.5, -3.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in a.
        Coordinate c(1.50, -2.5, 2.0);
        const Coordinate ref(-1.50, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in a.
        Coordinate c(-1.51, -2.5, 2.0);
        const Coordinate ref(1.49, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in b.
        Coordinate c(-1.47, 2.50, 2.0);
        const Coordinate ref(-1.47, -2.5, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);

    }
    {
        // Should be wrapped in b.
        Coordinate c(-1.47, -4.30, 2.0);
        const Coordinate ref(-1.47, 0.70, 2.0);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in c.
        Coordinate c(-1.47, 0.70, 3.001);
        const Coordinate ref(-1.47, 0.70, -2.999);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
    {
        // Should be wrapped in c.
        Coordinate c(-1.47, 0.70, -3.001);
        const Coordinate ref(-1.47, 0.70, 2.999);
        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref.z(), c.z(), 1.0e-14);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeMap::testWrapLong()
{
    // Wrapping longer than one cell is not needed and will not be
    // supported now. This test checks the expected behaviour.

    // Construct a periodic map.
    std::vector<int> repetitions(3);
    repetitions[0] = 3;
    repetitions[1] = 5;
    repetitions[2] = 6;
    const std::vector<bool> periodicity(3, true);
    LatticeMap map(3, repetitions, periodicity);

    {
        // Should be wrapped in all directions.
        Coordinate c(1.5+3.0, 2.5+5.0, 3.0+12.0);

        // If wrapping more than one box length was supported this
        // would be the correct reference.
        const Coordinate ref(-1.5, -2.5, -3.0);

        // Now this is what we expect to get instead.
        const Coordinate ref2(1.5, 2.5, 9.0);

        // Wrap and check.
        map.wrap(c);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.x(), c.x(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.y(), c.y(), 1.0e-14);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ref2.z(), c.z(), 1.0e-14);
    }
}