#include "flthp.hpp"

HighpassFilter::HighpassFilter(double fc, double fs, int order)
    : m_fc(fc)
{
    m_fs = fs;
    m_order = order;
    m_coef = butterworth_coefficients(order, fc, fs);
    m_w.resize(3);
    for(int i = 0; i < 3; i++) m_w[i].resize(m_order / 2);
};


std::vector<double**>
HighpassFilter::butterworth_coefficients(int order, double fc, double fs)
{
    if(order % 2)
        order++;
    static std::vector<double**> coefficients(order / 2);
    for(int i = 0; i < order / 2; i++)
    {
        coefficients[i] = new double*[2];
        for(int j = 0; j < 2; j++)
            coefficients[i][j] = new double[3];
    }

    double a = std::tan(M_PI * fc / fs);
    for(int i = 0; i < order / 2; i++)
    {
        double r = std::sin(M_PI * (2. * i + 1.) / (2.0 * order));
        double s = a * a + 2 * a * r + 1;
        //a0, a1, a2
        coefficients[i][0][0] = 1;
        coefficients[i][0][1] = -2.0 * (1 - a * a) / s;
        coefficients[i][0][2] = (a * a - 2.0 * a * r + 1.0) / s;
        //b0, b1, b2
        coefficients[i][1][0] = 1 / s;
        coefficients[i][1][1] = - 2.0 / s;
        coefficients[i][1][2] = 1 / s;
    }
    return coefficients;
}