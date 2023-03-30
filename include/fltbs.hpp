#ifndef __FLTBS_HPP__
#define __FLTBS_HPP__

#include "fltlib.hpp"

class BandstopFilter : public Filter
{
    public:
    BandstopFilter(double low, double high, double fs, int order);
    ~BandstopFilter(){};

     /**
     * @brief Apply the filter to the data. The data sampling rate is assumed to be constant and equal to the sampling rate used to initialize the filter
     *
     * @param data_src The data to be filtered
     * @param data_dst The filtered data
     * @param init If true, the filter will be initialized. If false, the filter will be applied to the data with the previous state
     */
    void
    apply(std::vector<double> &data_src,
          std::vector<double> &data_dst,
          bool init);

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
    double m_order;
    std::vector<double**> m_coef;//ab
    std::vector<std::vector<double>> m_w;
};

#endif // __FLTBS_HPP__