#include <bits/stdc++.h>
#include <signal.h>

#include "Simulator.hpp"
#include "GetTypes.hpp"
#ifndef TYPES
#error No defined Types (You can add them in CMakeLists.txt)
#endif

std::unique_ptr<Base> sim;
size_t sim_index;


void my_handler(int signo){
    sim->save();
    exit(0);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, my_handler);
    ParseTypes command_line(argc, argv);
    Field read_file(command_line.inputfile, command_line.outputfile);

    std::tuple need = {command_line.p, command_line.v, command_line.vf, read_file.N, read_file.M};

    sim_index = std::find(types.begin(), types.end(), need) - types.begin();
    if (sim_index == types.size()){
        need = {command_line.p, command_line.v, command_line.vf, 0, 0};
        sim_index = std::find(types.begin(), types.end(), need) - types.begin();
        if (sim_index == types.size()) {
            std::cout << "Simulator with chosen types does not exist\n"; exit(EXIT_FAILURE);
        }
    }

    sim = simulators[sim_index]();
    sim->build(command_line.inputfile, command_line.outputfile);
    sim->run();

    return 0;
}
