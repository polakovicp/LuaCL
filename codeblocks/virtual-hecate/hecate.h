#ifndef HECATE_HPP
#define HECATE_HPP

#include "log.hpp"

#include <iostream>
#include <fstream>

namespace Hecate
{
    class VHecate
    {
    public:
        VHecate() {};
        VHecate(std::istream& in);
    };
};

void ReadEvents(const char * log);
#endif // HECATE_HPP
