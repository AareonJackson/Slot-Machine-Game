#include <iostream>

void runSaveManagerTests();
void runPayoutTests();

int main() {
    std::cout << "Running SlotMachine tests...\n";
    
    runSaveManagerTests();
    runPayoutTests();

    std::cout << "All tests passed!\n";
    return 0;
}