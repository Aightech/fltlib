#ifndef __FLTLP_HPP__
#define __FLTLP_HPP__

#include "fltlib.hpp"

class LowpassFilter : public Filter
{
    public:
    LowpassFilter(double fc, double fs, int order);
    LowpassFilter(double fc, double fs, double ep, int order);
    ~LowpassFilter(){};

    static std::vector<double **>
    butterworth_coefficients(int order, double fc, double fs);

    static std::vector<double **>
    chebyshev_coefficients(int order, double fc, double ep, double fs);

    private:
    double m_fc;
};

#endif // __FLTLP_HPP__