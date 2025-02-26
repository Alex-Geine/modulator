#include "generator.h"
#include "file_writer.h"

int main()
{
    std::vector<int16_t> data;          // Data from generator
    std::vector<double>  time_p;        // Time points data

    double   freq        = 110;         // Frequency of signal
    double   sample_freq = 1000;        // Sample frequensy of signal
    uint64_t N           = 1000;        // Number of points
    double   phase       = 0;           // Phase of generated signal in 

    data.resize(N);
    time_p.resize(N);

    for(uint64_t i = 0; i < N; ++i)
        time_p[i] = i;

    std::cout << "Type freq, sample_freq, phase (deg), N:" << std::endl;
    std::cin >> freq >> sample_freq >> phase >> N;

    // Convert into radians
    phase = pi2 /  360.;

    // Generate data

    Generator gen(freq, sample_freq);

    gen.SetPhase(phase);
    gen.GenData(data);

    // Write to file
    writeToFile(time_p, data, "gen_data.txt");    

    return 0;
}