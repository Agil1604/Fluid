#include <bits/stdc++.h>

#include "Simulator.hpp"
// #include "GetTypes.hpp"

constexpr size_t cringe_N = 36, cringe_M = 84;

int main(int argc, char* argv[]) {
    #ifdef TYPES
    // ParseTypes command_line(argc, argv);
    // Field read_file(command_line.inputfile);
    Simulator<FastFixed<64, 8>, FastFixed<64, 8>, FastFixed<64, 8>, cringe_N, cringe_M> x("../input.txt");
    x.run();
    
    #else
    std::cout << "No defined Types\n";
    #endif
    return 0;
}
