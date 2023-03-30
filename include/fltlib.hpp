#ifndef __FLTLIB_HPP__
#define __FLTLIB_HPP__

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// Virtual base class for filters
class Filter
{

    public:
    Filter(){};
    ~Filter(){};

    /**
     * @brief Apply the filter to the data. The data sampling rate is assumed to be constant and equal to the sampling rate used to initialize the filter
     *
     * @param data_src The data to be filtered
     * @param data_dst The filtered data
     * @param init If true, the filter will be initialized. If false, the filter will be applied to the data with the previous state
     */
    virtual void
    apply(std::vector<double> &data_src,
          std::vector<double> &data_dst,
          bool init = true) = 0;

    /**
     * @brief Apply the filter to the data. Use this function if the data sampling rate is not constant. The data will be resampled to the sampling rate used to initialize the filter
     * 
     * @param data_src The data to be filtered
     * @param data_dst The filtered data
     * @param timestamps The timestamps of the data
     * @param init True if the filter should be initialized, false otherwise
     * @param reresample True if the filtered data should be resampled to the sampling rate used to initialize the filter, false otherwise
     */
    void
    apply(std::vector<double> &data_src,
          std::vector<double> &data_dst,
          std::vector<double> &timestamps,
          bool init, bool reresample = false);

    static double
    interpolate(double x, double x1, double y1, double x2, double y2);

    static void
    resample(const std::vector<double> &data_src,
             std::vector<double> &data_dst,
             const std::vector<double> &timestamps,
             double sampling_rate);

    protected:
    double m_fs;
};

#include "fltbp.hpp"
#include "fltlp.hpp"
#include "fltbs.hpp"
#include "flthp.hpp"



#endif /* __FLTLIB_HPP__ */
