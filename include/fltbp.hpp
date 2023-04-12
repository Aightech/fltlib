#ifndef __FLTBP_HPP__
#define __FLTBP_HPP__

#include "fltlib.hpp"

class Bandpass : public Filter
{
    public:
    Bandpass(double low, double high, double fs, int order);
    ~Bandpass(){};

    /**
     * @brief Compute the coefficients of a Butterworth bandpass filter
     *
     * @param order The order of the filter
     * @param low The lower cutoff frequency
     * @param high The higher cutoff frequency
     * @param fs The sampling rate
     * @return std::vector<std::vector<double>>  The coefficients of the filter
     */
    static std::vector<double **>
    butterworth_coefficients(int order, double low, double high, double fs);

    private:
    Bandpass(double fs, int order);

    double m_low;
    double m_high;
};

#endif // __FLTBP_HPP__