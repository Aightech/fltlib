#include "fltbs.hpp"

Bandstop::Bandstop(double fs, int order)
{
    m_name = "Bandstop";
    m_fs = fs;
    //get the closest multiple of 4    
    if(order % 4!=0)
        order += 4 - order % 4;
    m_order = order;
    m_w.resize(5);
    for(int i = 0; i < 5; i++) m_w[i].resize(m_order / 4);
};

Bandstop::Bandstop(double low, double high, double fs, int order)
    : Bandstop(fs, order)
{
    m_low = low;
    m_high = high;
    m_coef = butterworth_coefficients(order, low, high, fs);
};

std::vector<double **>
Bandstop::butterworth_coefficients(int order,
                                         double low,
                                         double high,
                                         double fs)
{
    if(order % 4)
        order += 4 - order % 4;

    static std::vector<double **> coefficients(order / 4);
    for(int i = 0; i < order / 4; i++)
    {
        coefficients[i] = new double *[2];
        for(int j = 0; j < 2; j++) coefficients[i][j] = new double[5];
    }

    double a =
        std::cos(M_PI * (high + low) / fs) / std::cos(M_PI * (high - low) / fs);
    double b = std::tan(M_PI * (high - low) / fs);

    for(int i = 0; i < order / 4; i++)
    {
        double r = std::sin(M_PI * (2. * i + 1.) / order);
        double s = b * b + 2 * b * r + 1;
        // a0, a1, a2, a3, a4
        coefficients[i][0][0] = 1;
        coefficients[i][0][1] = -4.0 * a * (1.0 + b * r) / s;
        coefficients[i][0][2] = -2.0 * (b * b - 2.0 * a * a - 1.0) / s;
        coefficients[i][0][3] = -4.0 * a * (1.0 - b * r) / s;
        coefficients[i][0][4] = (b * b - 2.0 * b * r + 1.0) / s;

        r = 4.0 * a;
        double s2 = 4.0 * a * a + 2.0;
        // b0, b1, b2, b3, b4
        coefficients[i][1][0] = 1 / s;
        coefficients[i][1][1] = -r / s;
        coefficients[i][1][2] = s2/s;
        coefficients[i][1][3] = -r / s;
        coefficients[i][1][4] = 1 / s;
    }
    return coefficients;
}