#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>



const double pi2 = 4 * atan(1) * 2;

#define AMPL 0x7FFF

class Generator
{
    private:

    double m_phase       = 0;
    double m_freq        = 0;
    double m_sample_freq = 0;
    double m_volume      = 1;

    public:

    // Constructor
    Generator(double freq = 110, double sample_freq = 1000)
             : m_freq(freq), m_sample_freq(sample_freq){};

    // Gen data
    void GenData(std::vector<int16_t>& out_data);

    // Set phase
    void SetPhase(double phase);
};

#endif // _GENERATOR_H_