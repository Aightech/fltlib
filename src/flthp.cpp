#include "flthp.hpp"

HighpassFilter::HighpassFilter(double fc, double fs, int order)
    : m_fc(fc), m_order(order)
{
    m_fs = fs;
    m_coef = butterworth_coefficients(order, fc, fs);
    m_w.resize(3);
    for(int i = 0; i < 3; i++) m_w[i].resize(m_order / 2);
};

void
HighpassFilter::apply(std::vector<double> &data_src,
                     std::vector<double> &data_dst,
                     bool init)
{
    //check if dst has the same size as src
    if(data_dst.size() != data_src.size())
        data_dst.resize(data_src.size());

    if(init)
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < m_order / 2; j++) m_w[i][j] = 0;

    // Apply filter
    for(int i = 0; i < data_src.size(); ++i)
    {
        for(int k = 0; k < m_order / 2; k++)
        {
            //T(w(k)/x(k)) = a(0) + a(1)*z^-1 + a(2)*z^-2
            m_w[0][k] = m_coef[k][0][0] * +data_src[i] -
                        m_coef[k][0][1] * m_w[1][k] -
                        m_coef[k][0][2] * m_w[2][k];
            //T(y(k)/w(k)) = b(0) + b(1)*z^-1 + b(2)z^-2
            data_dst[i] = m_coef[k][1][0] * m_w[0][k] +
                          m_coef[k][1][1] * m_w[1][k] +
                          m_coef[k][1][2] * m_w[2][k];
            m_w[2][k] = m_w[1][k];
            m_w[1][k] = m_w[0][k];
        }
    }
}

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