#include "Field.hpp"
#include <iostream>
#include <fstream>
#include <string>

Field::Field(std::string filename){
    path = filename;
    get_field();
}

void Field::get_field(){
    std::ifstream input (path);
    std::string myline;

    if (input.is_open()){
        double G;
        input >> N >> M >> g >> count;
        
        input.get();

        for (int i = 0; i < count; ++i){
            char tmp = input.get();
            input >> rho[tmp];
            input.get();
        }
        
        T = 0;
        while (!input.eof()){
            std::getline(input, myline);
            if (myline.length() != 0){
                field.emplace_back(myline);
            }
        }
    }
    input.close();
}

void Field::print_field(){
    for (auto line : field){
        std::cout << line << std::endl;
    }
}

void Field::print_field(std::string out){
    std::ofstream outfile (out);
    outfile << N << " " << M << std::endl;
    outfile << "Tick: " << T << std::endl;
    for (const auto& line : field){
        outfile << line << std::endl;
    }
}
