#include "core/rng.h"
#include <cassert>
#include <iostream>
#include <vector>

void runRngTests() {
    std::cout << "Running RNG tests...\n";

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

    {
        RNG rng;

        for (int i = 0; i < 100; ++i) {
            int value = rng.getRandomInt(10, 5);
            assert(value >= 5);
            assert(value <= 10);
        }
    }

    {
        RNG rng;

        int value = rng.getRandomInt(7, 7);
        assert(value == 7);
    }

    std::cout << "RNG tests passed!\n";
}
