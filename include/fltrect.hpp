#ifndef __FLTRECT_HPP__
#define __FLTRECT_HPP__

#include "fltlib.hpp"

class Rectifier: public Filter
{
    public:
    Rectifier(bool double_sided = false)
    {
        m_name = std::string("Rectifier") + (double_sided ? " (double sided)" : " (single sided)");
        if (double_sided)
        this->set_filtering_function(
            [](double x) -> double
            {
                return std::abs(x);
            }
        );
        else
        this->set_filtering_function(
            [](double x) -> double
            {
                return std::max(x, 0.0);
            }
        );
        
        
    };
    ~Rectifier(){};

    
};

#endif // __FLTRECT_HPP__