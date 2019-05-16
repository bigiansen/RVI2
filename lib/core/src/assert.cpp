#include <rvi/assert.hpp>

#include <stdexcept>
#include <iostream>

namespace rvi
{
    void r_assert(bool cond, const std::string& msg)
    {
        std::cerr << msg << std::endl;
        if(!cond)        
            throw std::logic_error(msg);
    }

    void debug_assert(bool cond, const std::string& msg)
    {
        #ifdef DEBUG
            r_assert(cond, msg);
        #endif
    }
}