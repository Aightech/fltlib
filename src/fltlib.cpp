#include "fltlib.hpp"

double
Filter::interpolate(double x, double x1, double y1, double x2, double y2)
{
    return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
};

void
Filter::resample(const std::vector<double> &data_src,
                 std::vector<double> &data_dst,
                 const std::vector<double> &timestamps,
                 double sampling_rate)
{
    double start_time = timestamps.front();
    double end_time = timestamps.back();
    double dt = 1 / sampling_rate;

    //clear dst
    data_dst.clear();

    for(double t = start_time; t <= end_time; t += dt)
    {
        auto it = std::upper_bound(timestamps.begin(), timestamps.end(), t);
        if(it == timestamps.begin())
        {
            data_dst.push_back(data_src.front());
        }
        else if(it == timestamps.end())
        {
            data_dst.push_back(data_src.back());
        }
        else
        {
            size_t index = std::distance(timestamps.begin(), it);
            double interpolated_value =
                interpolate(t, timestamps[index - 1], data_src[index - 1],
                            timestamps[index], data_src[index]);
            data_dst.push_back(interpolated_value);
        }
    }
};

void
Filter::apply(std::vector<double> &data_src,
                      std::vector<double> &data_dst,
                      bool init)
{
    //check if dst has the same size as src
    if(data_dst.size() != data_src.size())
        data_dst.resize(data_src.size());

    if(init)
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < m_order / 4; j++) m_w[i][j] = 0;
    // Apply filter
    for(int i = 0; i < data_src.size(); ++i)
    {
        for(int n = 0; n < m_order / 4; n++)
        {
            m_w[0][n] = m_coef[n][0][0] * data_src[i];
            for(int k = 1; k < m_w.size(); k++)
                m_w[0][n] -= m_coef[n][0][k] * m_w[k][n];
            data_dst[i]=0;
            for(int k = 1; k < m_w.size(); k++)
                data_dst[i] += m_coef[n][1][k] * m_w[k][n];
            for(int k = m_w.size()-1; k > 0; k--)
                m_w[k][n] = m_w[k-1][n];
        }
    }
}

void
Filter::apply(std::vector<double> &data_src,
              std::vector<double> &data_dst,
              std::vector<double> &timestamps,
              bool init,
              bool reresample)
{
    // Interpolate data to a uniform grid
    resample(data_src, data_dst, timestamps, init);
    // Apply bandpass filter on the resampled data
    apply(data_dst, data_dst, m_fs);

    if(resample) // Resample the filtered data to the original sampling rate
        resample(data_dst, data_dst, timestamps, m_fs);
}


