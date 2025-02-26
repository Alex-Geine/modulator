#include "modulator.h"

// Gen data
void Modulator::GenData(const std::vector<uint8_t>& info_bits,
                        std::vector<int16_t>& out_data)
{
    uint32_t N = out_data.size();
    uint32_t n_per_bit = N / info_bits.size();
    double d_phase = m_freq * pi2 / m_sample_freq;
    int counter = 0;

    for(uint64_t i = 0; i < N; i++)
    {
        if (!(i % n_per_bit))
        {
            m_phase = pi2 / 2 * info_bits[counter];
            counter++;
        }
        out_data[i]  = sin(m_phase) * AMPL * m_volume;
        m_phase += d_phase;
        if (m_phase >= pi2)
            m_phase -= pi2;
    }

    return;
};

// Set phase
void Modulator::SetPhase(double phase)
{
    m_phase = phase;

    return;
};