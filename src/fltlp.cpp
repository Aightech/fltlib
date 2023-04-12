#include "fltlp.hpp"

Lowpass::Lowpass(double fs, int order)
{
    m_name = "Lowpass";
    m_fs = fs;
    m_order = order;
    m_w.resize(3);
    for(int i = 0; i < 3; i++) m_w[i].resize(m_order / 2);
};

Lowpass::Lowpass(double fc, double fs, int order) : Lowpass(fs, order)
{
    m_fc=fc;
    m_coef = butterworth_coefficients(order, fc, fs);
};

Lowpass::Lowpass(double fc, double ep, double fs, int order)
    : Lowpass(fs, order)
{
    m_fc = fc;
    m_coef = chebyshev_coefficients(order, fc, ep, fs);
};

std::vector<double **>
Lowpass::butterworth_coefficients(int order, double fc, double fs)
{
    if(order % 2)
        order++;
    static std::vector<double **> coefficients(order / 2);
    for(int i = 0; i < order / 2; i++)
    {
        coefficients[i] = new double *[2];
        for(int j = 0; j < 2; j++) coefficients[i][j] = new double[3];
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
        coefficients[i][1][0] = a * a / s;
        coefficients[i][1][1] = 2.0 * a * a / s;
        coefficients[i][1][2] = a * a / s;
    }
    return coefficients;
}

std::vector<double **>
Lowpass::chebyshev_coefficients(int order,
                                      double fc,
                                      double ep,
                                      double fs)
{
    if(order % 2)
        order++;
    static std::vector<double **> coefficients(order / 2);
    for(int i = 0; i < order / 2; i++)
    {
        coefficients[i] = new double *[2];
        for(int j = 0; j < 2; j++) coefficients[i][j] = new double[3];
    }

    double a = std::tan(M_PI * fc / fs);
    double u = std::log((1.0 + std::sqrt(1.0 + ep * ep)) / ep);
    double su = sinh(u / order);
    double cu = cosh(u / order);
    for(int i = 0; i < order / 2; i++)
    {
        double b = sin(M_PI * (2.0 * i + 1.0) / (2.0 * order)) * su;
        double c = cos(M_PI * (2.0 * i + 1.0) / (2.0 * order)) * cu;
        c = b * b + c * c;
        double s = a * a * c + 2.0 * a * b + 1.0;
        //a0, a1, a2
        coefficients[i][0][0] = 1;
        coefficients[i][0][1] = -2.0 * (1 - a * a * c) / s;
        coefficients[i][0][2] = (a * a * c - 2.0 * a * b + 1.0) / s;
        //b0, b1, b2
        coefficients[i][1][0] = a * a / (4 * s) * 2 / ep;
        coefficients[i][1][1] = 2.0 * a * a / (4 * s) * 2 / ep;
        coefficients[i][1][2] = a * a / (4 * s) * 2 / ep;
    }
    return coefficients;
}