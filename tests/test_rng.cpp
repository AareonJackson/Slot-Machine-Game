#include "core/rng.h"
#include <cassert>
#include <iostream>
#include <vector>

/*
 * @Purpose: Verifies the RNG class produces valid bounded values for slot reel stops and
 * general random integer requests. These tests confirm that generated stop
 * positions match the number of provided reels, stay within each reel's valid
 * index range, gracefully handle invalid reel lengths by returning 0, normalize
 * reversed min/max bounds, and correctly handle identical bounds.
 */

void runRngTests() {
    std::cout << "Running RNG tests...\n";
    // Unit Test Case 1: generateStops with valid reel lengths
    {
        RNG rng;
        std::vector<int> reelLengths = {5, 10, 20};

        std::vector<int> stops = rng.generateStops(reelLengths);

        assert(stops.size() == reelLengths.size());

        for (std::size_t i = 0; i < stops.size(); ++i) {
            assert(stops[i] >= 0);
            assert(stops[i] < reelLengths[i]);
        }
    }
    // Unit Test Case 2: generateStops with invalid reel lengths returns 0
    {
        RNG rng;
        std::vector<int> reelLengths = {0, -5, 3};

        std::vector<int> stops = rng.generateStops(reelLengths);

        assert(stops.size() == reelLengths.size());
        assert(stops[0] == 0);
        assert(stops[1] == 0);
        assert(stops[2] >= 0);
        assert(stops[2] < 3);
    }
    // Unit Test Case 3: getRandomInt result is within range; if min > max, it still returns within corrected range
    {
        RNG rng;

        for (int i = 0; i < 100; ++i) {
            int value = rng.getRandomInt(10, 5);
            assert(value >= 5);
            assert(value <= 10);
        }
    }
    // Unit Test Case 4: getRandomInt result is within range; if min == max, it returns the same value
    {
        RNG rng;

        int value = rng.getRandomInt(7, 7);
        assert(value == 7);
    }

    std::cout << "RNG tests passed!\n";
}
