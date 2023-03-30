#ifndef __FLTHP_HPP__
#define __FLTHP_HPP__

#include "fltlib.hpp"

class HighpassFilter : public Filter
{
    public:
    HighpassFilter(double fc, double fs, int order);
    ~HighpassFilter(){};

    static std::vector<double**>
    butterworth_coefficients(int order, double fc, double fs);

    private:
    double m_fc;
};

#endif // __FLTHP_HPP__