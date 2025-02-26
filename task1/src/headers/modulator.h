#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

const double pi2 = 4 * atan(1) * 2;

#define AMPL 0x7FFF

class Modulator
{
    private:

    double m_phase       = 0;
    double m_freq        = 0;
    double m_sample_freq = 0;
    double m_volume      = 1;

    public:

    // Constructor
    Modulator(double freq = 110, double sample_freq = 1000)
             : m_freq(freq), m_sample_freq(sample_freq){};

    // Gen data
    void GenData(const std::vector<uint8_t>& info_bits,
                 std::vector<int16_t>& out_data);

    // Set phase
    void SetPhase(double phase);
};