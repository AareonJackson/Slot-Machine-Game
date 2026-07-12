#pragma once
#include <vector>
#include <random>

class RNG {
public:
    // Initialize the RNG with a safe seed
    RNG() = default;

    // Ensure an uncorrupted state of randomness
    // Delete copy constructors
    RNG(const RNG&) = delete;
    RNG& operator=(const RNG&) = delete;

    /*
     * Generate a random stop position for each reel
     * @param reelLengths A vector containing the length (number of symbols) of each reel strip.
     * @return A vector of stopping indices corresponding to each reel.
    */
    std::vector<int> generateStops(const std::vector<int>& reelLengths);

    /*
     * General utility for getting a random integer.
     * @param min Minimum bound (inclusive)
     * @param max Maximum bound (inclusive)
    */
    int getRandomInt(int min, int max);

private:
    std::random_device m_cryptoRng; // Cryptographically secure random number generator
};