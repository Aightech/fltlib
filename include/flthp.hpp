#ifndef __FLTHP_HPP__
#define __FLTHP_HPP__

#include "fltlib.hpp"

class Highpass : public Filter
{

    public:
    Highpass(double fc, double fs, int order);
    ~Highpass(){};

    static std::vector<double**>
    butterworth_coefficients(int order, double fc, double fs);

    private:
    Highpass(double fs, int order);
    double m_fc;
};

#endif // __FLTHP_HPP__