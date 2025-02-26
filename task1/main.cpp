#include <fstream>
#include "modulator.h"

int main()
{
    std::ofstream file("data.txt");

    std::vector<int16_t> data;
    std::vector<double> time_p;
    std::vector<uint8_t> info_bits = {1,1,0,1,1,0,1,1,0,1,1};
    double freq = 110;
    double sample_freq = 1000;
    uint64_t N = 1000;
    double phase = 0;

    data.resize(N);
    time_p.resize(N);

    std::cout << "Type freq, sample_freq, N, volume_level:" << std::endl;
    std::cin >> freq >> sample_freq >> N;

    Modulator mod(freq, sample_freq);

    mod.SetPhase(phase);
    mod.GenData(info_bits, data);

    for(uint64_t i = 0; i < N; ++i)
        time_p[i] = i;

    for (uint64_t i = 0; i < N; ++i)
        file << time_p[i] << " " << data[i] << "\n";

    return 0;
}