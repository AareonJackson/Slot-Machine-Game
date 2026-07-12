#include <iostream>

void runSaveManagerTests();
void runPayoutTests();
void runRNGTests();

int main() {
    std::cout << "Running SlotMachine tests...\n";
    
    runSaveManagerTests();
    runPayoutTests();
    runRNGTests();

    std::cout << "All tests passed!\n";
    return 0;
}