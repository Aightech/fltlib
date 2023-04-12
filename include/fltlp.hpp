#ifndef __FLTLP_HPP__
#define __FLTLP_HPP__

#include "fltlib.hpp"

class Lowpass : public Filter
{
    public:
    Lowpass(double fc, double fs, int order);
    Lowpass(double fc, double fs, double ep, int order);
    ~Lowpass(){};

    static std::vector<double **>
    butterworth_coefficients(int order, double fc, double fs);

    static std::vector<double **>
    chebyshev_coefficients(int order, double fc, double ep, double fs);

    private:
    Lowpass(double fs, int order);
    double m_fc;
};

#endif // __FLTLP_HPP__