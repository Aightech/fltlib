#ifndef __FLTLP_HPP__
#define __FLTLP_HPP__

#include "fltlib.hpp"

class LowpassFilter : public Filter
{
    public:
    LowpassFilter(double fc, double fs, int order);
    ~LowpassFilter(){};
    void
    apply(std::vector<double> &data_src,
          std::vector<double> &data_dst,
          bool init);

    static std::vector<double**>
    butterworth_coefficients(int order, double fc, double fs);

    private:
    double m_fc;
    double m_order;
    std::vector<double**> m_coef;//ab
    std::vector<std::vector<double>> m_w;
};

#endif // __FLTLP_HPP__