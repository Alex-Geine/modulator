#include "generator.h"

// GenData - function for generating signal
/* 
    [out] out_data - output data
    return void
*/
void Generator::GenData(std::vector<int16_t>& out_data)
{
    uint32_t N = out_data.size();
    double d_phase = m_freq * pi2 / m_sample_rate;

    for(uint64_t i = 0; i < N; i++)
    {
        out_data[i] = sin(m_phase) * AMPL * m_volume;
        m_phase += d_phase;
        if (m_phase >= pi2)
            m_phase -= pi2;
    }

    return;
};

// SetPhase - function for setting phase value
/*
    [in] phase - value of phase to set
    return void
*/
void Generator::SetPhase(double phase)
{
    m_phase = phase;

    return;
};