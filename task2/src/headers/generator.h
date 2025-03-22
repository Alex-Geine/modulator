#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

// 2 * PI constant
const double pi2 = 4 * atan(1) * 2;

// Maximal amplitude of signal as 16-bit signed value
#define AMPL 0x7FFF

// Class of signal generator
class Generator
{
    public:

    double m_phase       = 0;   // Initial phase
    double m_freq        = 0;   // Signal frequency
    double m_sample_rate = 0;   // Sample rate
    double m_volume      = 1;   // Level of signal amplitude

    public:

    // Constructor
    Generator(double freq = 110, double sample_rate = 1000): m_freq(freq), m_sample_rate(sample_rate){};

    // GenData - function for generating signal
    void GenData(std::vector<int64_t>& out_data);

    // SetPhase - function for setting phase value
    void SetPhase(double phase);
};

#endif // _GENERATOR_H_