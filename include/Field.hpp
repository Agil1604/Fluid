#ifndef FIELD_
#define FIELD_

#include "Fixed.hpp"
#include <vector>
#include <string>

class Field
{
public:
    Field() = default;
    Field(std::string filename);
    void get_field();
    void print_field();
    void print_field(std::string filename);

    std::vector<std::string> field;
    int N;
    int M;
    double g;
    int T;
    int count;
    double rho[256];

private:
    std::string path;
};

#endif