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
    /**
     * @brief Interpolate the value base on the two points (x1,y1) and (x2,y2)
     *
     * @param x The abscissa of the point to be interpolated
     * @param x1 The abscissa of the first point
     * @param y1 The ordinate of the first point
     * @param x2 The abscissa of the second point
     * @param y2 The ordinate of the second point
     * @return double The interpolated value at the abscissa x based on the two points (x1,y1) and (x2,y2)
     */
    static double
    interpolate(double x, double x1, double y1, double x2, double y2);

    static void
    resample(const std::vector<double> &data_src,
             std::vector<double> &data_dst,
             const std::vector<double> &timestamps,
             double sampling_rate);

    public:
    Filter(){};
    ~Filter(){};
    /**
     * @brief Apply the filter to the data. Use this function if the data sampling rate is constant and equal to the sampling rate used to initialize the filter
     * @param value The value to be filtered
     * @return double The filtered value
     */
    double
    apply(double value);

    double
    default_filtering_function(double value);

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
          bool init = true);

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
          bool init,
          bool reresample = false);

    /**
     * @brief Apply the filter to the data. Use this function if the data sampling rate is not constant. The data will be resampled to the sampling rate used to initialize the filter
     *
     * @param value The value to be filtered
     * @param timestamp The timestamp of the value
     * @return double The filtered value at the timestamp=last_timestamp+n/fs . n = 1 if the timestamp is between last_timestamp and last_timestamp+1/fs. else n is the integer part of (timestamp-last_timestamp)*fs
     */
    double
    apply(double value,
          double timestamp,
          std::vector<double> *data_dst = nullptr);

    void
    print_coefficients();

    void set_filtering_function(double (*filtering_function)(double)){
        m_filtering_function = filtering_function;
    }

    protected:
    double m_fs;
    double m_order;
    std::vector<double **> m_coef;
    std::vector<std::vector<double>> m_w;
    double m_last_timestamp;
    double m_last_value;
    double m_n;
    //filtering function pointer
    double (*m_filtering_function)(double) = nullptr;
    std::string m_name;
};

#include "fltbp.hpp"
#include "fltbs.hpp"
#include "flthp.hpp"
#include "fltlp.hpp"
#include "fltrect.hpp"

#endif /* __FLTLIB_HPP__ */
