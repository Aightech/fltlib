#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fltlib.hpp"

int
main(int argc, char **argv)
{
    //generate a vector of 1000 elements
    int n = 1000;
    std::vector<double> data_raw(n);
    std::vector<double> data_filtered(n);
    std::vector<double> timestamps(n);

    //fill the vector with a signal with a sinusoidal of 10Hz and a other sinusoidal of 100Hz
    //each time step is 1ms
    double dt = 0.001;
    double f1 = 10;
    double f2 = 100;
    double fs = 1 / dt;
    for(int i = 0; i < n; ++i)
    {
        timestamps[i] = i * dt;
        data_raw[i] = sin(2 * M_PI * f1 * timestamps[i]) + sin(2 * M_PI * f2 * timestamps[i]);
    }

    //Filter *filter = new BandpassFilter(20, 80, fs, 4);
    //Filter *filter = new LowpassFilter(50, fs, 2);
    //Filter *filter = new BandstopFilter(99, 101, fs, 4);
    Filter *filter = new HighpassFilter(50, fs, 2);
    filter->apply(data_raw, data_filtered, fs);

    //write the data to a file timestamp, raw, filtered
    FILE *fp = fopen("data.txt", "w");
    for(int i = 0; i < n; ++i)
    {
        fprintf(fp, "%f %f %f\n", timestamps[i], data_raw[i], data_filtered[i]);
    }
    fclose(fp);

    //plot [][-2:2]"data.txt" u 1:2 w lines, "data.txt" u 1:3 w lines

    
    return 0;
}
