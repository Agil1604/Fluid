#ifndef FIELD_
#define FIELD_

#include "Fixed.hpp"
#include <vector>
#include <string>

class Field
{
public:
    Field() = default;
    Field(std::string in, std::string out);
    void get_field();
    void print_field();
    void save_field();

    std::vector<std::string> field;
    int N;
    int M;
    double g;
    int count;
    double rho[256];

private:
    std::string infile;
    std::string outfile;
    std::vector<char> types{};
};

#endif