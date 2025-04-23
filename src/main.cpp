#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fltlib.hpp"

//gnuplot -p -e "plot 'data.txt' u 1:2 w lines, 'data.txt' u 1:3 w lines"

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
        data_raw[i] = sin(2 * M_PI * f1 * timestamps[i]) + sin(2 * M_PI * f2 * timestamps[i]) + sin(2 * M_PI * 50 * timestamps[i]);
    }
    Filter *filter;
    int i = atoi(argv[1]);
    switch (i)
    {
        case 0:
            filter = new Lowpass(50, fs, 20);
            break;
        case 1:
            filter = new Highpass(50, fs, 20);
            break;
        case 2:
            filter = new Bandpass(49, 80, fs, 20);
            break;
        case 3:
            filter = new Bandstop(48, 52, fs, 40);
            break;
        case 4://rectify
            filter = new Rectifier(fs);
            break;
        default:
            filter = new Lowpass(50, fs, 2);
            break;
    }

    filter->print_coefficients();

    filter->apply(data_raw, data_filtered);


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
