#include <iostream>

void runSaveManagerTests();
void runPayoutTests();
void runRngTests();
void runConfigModelTests();

int main() {
    std::cout << "Running SlotMachine tests...\n";
    
    runSaveManagerTests();
    runPayoutTests();
    runRngTests();
    runConfigModelTests();

    std::cout << "All tests passed!\n";
    return 0;
}