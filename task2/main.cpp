#include <fstream>
#include "generator.h"

int main()
{
    std::ofstream file("gen_data.txt");

    std::vector<int16_t> data;
    std::vector<double>  time_p;

    double   freq        = 110;
    double   sample_freq = 1000;
    uint64_t N           = 1000;
    double   phase       = 0;           // Phase of generated signal in 

    data.resize(N);
    time_p.resize(N);

    std::cout << "Type freq, sample_freq, phase (deg), N:" << std::endl;
    std::cin >> freq >> sample_freq >> phase >> N;

    // Convert into radians
    phase = pi2 /  360.;

    Generator gen(freq, sample_freq);

    gen.SetPhase(phase);
    gen.GenData(data);

    for(uint64_t i = 0; i < N; ++i)
        time_p[i] = i;

    for (uint64_t i = 0; i < N; ++i)
        file << time_p[i] << " " << data[i] << "\n";

    return 0;
}