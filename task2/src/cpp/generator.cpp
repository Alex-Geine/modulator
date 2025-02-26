#include "generator.h"

// Gen data
void Generator::GenData(std::vector<int16_t>& out_data)
{
    uint32_t N = out_data.size();
    double d_phase = m_freq * pi2 / m_sample_freq;

    for(uint64_t i = 0; i < N; i++)
    {
        out_data[i] = sin(m_phase) * AMPL * m_volume;
        m_phase += d_phase;
        if (m_phase >= pi2)
            m_phase -= pi2;
    }

    return;
};

// Set phase
void Generator::SetPhase(double phase)
{
    m_phase = phase;

    return;
};