#include "fltlib.hpp"

double
Filter::interpolate(double x, double x1, double y1, double x2, double y2)
{ // Linear interpolation
    return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
};

void
Filter::resample2uniform(const std::vector<double> &data_src,
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
Filter::resample2original(const std::vector<double> &data_src,
                          std::vector<double> &data_dst,
                          double src_fs,
                          const std::vector<double> &timestamps)
{
    data_dst.clear();
    if(data_src.empty() || timestamps.empty() || src_fs <= 0.0)
        return;

    const double t0 = timestamps.front();
    const double dt = 1.0 / src_fs;
    const size_t N = data_src.size();
    data_dst.reserve(timestamps.size());

    for(double t : timestamps)
    {
        double p = (t - t0) * src_fs; // fractional index in data_src
        if(p <= 0.0)
        {
            data_dst.push_back(data_src.front());
        }
        else if(p >= double(N - 1))
        {
            data_dst.push_back(data_src.back());
        }
        else
        {
            size_t idx = size_t(std::floor(p));
            double t1 = t0 + idx * dt;
            double t2 = t1 + dt;
            double y1 = data_src[idx];
            double y2 = data_src[idx + 1];
            data_dst.push_back(interpolate(t, t1, y1, t2, y2));
        }
    }
}

double
Filter::apply(double value)
{

    if(m_filtering_function == nullptr)
        return default_filtering_function(value);
    else
        return m_filtering_function(value);
}

double
Filter::default_filtering_function(double value)
{
    for(size_t n = 0; n < m_w[0].size(); n++)
    {
        m_w[0][n] = m_coef[n][0][0] * value;
        for(size_t k = 1; k < m_w.size(); k++)
            m_w[0][n] -= m_coef[n][0][k] * m_w[k][n];
        value = 0;
        for(size_t k = 0; k < m_w.size(); k++)
            value += m_coef[n][1][k] * m_w[k][n];
        for(size_t k = m_w.size() - 1; k > 0; k--) m_w[k][n] = m_w[k - 1][n];
    }
    return value;
}

void
Filter::apply(const std::vector<double> &data_src,
              std::vector<double> &data_dst,
              bool init)
{
    //check if dst has the same size as src
    if(data_dst.size() != data_src.size())
        data_dst.resize(data_src.size());

    if(init)
        for(size_t i = 0; i < m_w.size(); i++)
            for(size_t j = 0; j < m_w[i].size(); j++) m_w[i][j] = 0;

    // Apply filter
    for(size_t i = 0; i < data_src.size(); ++i)
        data_dst[i] = apply(data_src[i]);
}

double
Filter::apply(double value, double timestamp, std::vector<double> *data_dst)
{
    int n = (timestamp - m_timestamp) / (1 / m_fs);
    for(int i = 0; i < n; i++)
    {
        m_value = interpolate(m_timestamp + 1 / m_fs, m_timestamp, m_value,
                              timestamp, value);
        m_timestamp += 1 / m_fs;
        m_last_filtered_value = m_filtered_value;
        m_filtered_value = apply(m_value);
        if(data_dst != nullptr)
            data_dst->push_back(m_filtered_value);
    }
    return interpolate(
        timestamp, m_timestamp - 1 / m_fs, m_last_filtered_value, m_timestamp,
        m_filtered_value); //return the filtered value interpolated at the coresponding timestamp
}

void
Filter::apply(const std::vector<double> &data_src,
              std::vector<double> &data_dst,
              const std::vector<double> &timestamps,
              bool init,
              bool reresample)
{
    // 1) uniform grid at m_fs
    std::vector<double> uniform;
    resample2uniform(data_src, uniform, timestamps, m_fs);

    // 2) in-place filter on uniform
    std::vector<double> filtered = uniform;
    apply(filtered, filtered, init);

    if(reresample)
    {
        // 3) project back to original timebase
        std::vector<double> original;
        resample2original(filtered, // src
                          original, // dst
                          m_fs,
                          timestamps // target times
        );
        data_dst = std::move(original);
    }
    else
    {
        // if no re-resample, return the uniform result
        data_dst = std::move(filtered);
    }
}

void
Filter::print_coefficients()
{
    printf("Filter: %s (order: %d, fs: %.2lf)\n", m_name.c_str(), (int)m_order,
           m_fs);
    for(size_t n = 0; n < m_coef.size(); n++)
    {
        printf("Stage: %ld\n", n);
        printf("a: [");
        for(size_t i = 0; i < m_w.size(); i++)
            printf("%.5lf\t", m_coef[n][0][i]);
        printf(" ]\n");
        printf("b: [");
        for(size_t i = 0; i < m_w.size(); i++)
            printf("%.5lf\t", m_coef[n][1][i]);
        printf(" ]\n");
    }
}