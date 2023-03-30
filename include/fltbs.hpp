#ifndef __FLTBS_HPP__
#define __FLTBS_HPP__

#include "fltlib.hpp"

class BandstopFilter : public Filter
{
    public:
    BandstopFilter(double low, double high, double fs, int order);
    ~BandstopFilter(){};

    /**
     * @brief Compute the coefficients of a Butterworth bandpass filter
     * 
     * @param order The order of the filter
     * @param low The lower cutoff frequency
     * @param high The higher cutoff frequency
     * @param fs The sampling rate
     * @return std::vector<std::vector<double>>  The coefficients of the filter
     */
    std::vector<double**>
    butterworth_coefficients(int order, double low, double high, double fs);

    private:
    double m_low;
    double m_high;
};

#endif // __FLTBS_HPP__