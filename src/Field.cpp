#include "Field.hpp"
#include <iostream>
#include <fstream>
#include <string>

Field::Field(std::string in, std::string out){
    infile = in;
    outfile = out;
    get_field();
}

void Field::get_field(){
    std::ifstream input (infile);
    std::string myline;

    if (input.is_open()){
        double G;
        input >> N >> M >> g >> count;
        
        input.get();

        for (int i = 0; i < count; ++i){
            char tmp = input.get();
            types.push_back(tmp);
            input >> rho[tmp];
            input.get();
        }
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


void Field::save_field(){
    std::ofstream out (outfile);
    out << N << " " << M << " " << g << " " << count << std::endl;
    for (auto& it : types){
        out << it << " " << rho[it] << std::endl;
    }
    out << std::endl;
    for (const auto& line : field){
        out << line << std::endl;
    }
    out.close();
    std::cout << std::endl << "All changes are saved in " << outfile << std::endl;
}
