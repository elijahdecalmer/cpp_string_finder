#include <iostream>
#include <array>

int main() {
    const int numWords = 1000000;
    const int avgWordLength = 5;

    int numNodes = numWords * avgWordLength;
    size_t nodeSize = 1 + 26 * sizeof(void*);

    size_t totalMemoryUsage = numNodes * nodeSize;

    std::cout << "Estimated Memory Usage: " << totalMemoryUsage << " bytes" << std::endl;

    return 0;
}