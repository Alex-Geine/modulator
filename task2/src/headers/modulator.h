#ifndef _MODULATOR_H_
#define _MODULATOR_H_

#include "generator.h"

// Class of signal generator
class Modulator : public Generator
{
    private:

    bool isFM2 = true;

    public:

    Modulator(double freq = 110, double sample_rate = 1000): Generator(freq, sample_rate){};
    // GenData - function for generating signal
    void GenData(const std::vector<uint8_t>& info_bits, std::vector<int64_t>& out_data)
    {
        uint64_t N = out_data.size();
        uint64_t info_bits_period = N / info_bits.size();
        double d_phase = m_freq * pi2 / m_sample_rate;

        //std::cout << "N: " << N << ", info_bits_periods: " << info_bits_period << ", d_phase: " << d_phase << std::endl;
        if (isFM2)
        {
             for(uint64_t i = 0; i < info_bits.size(); ++i)
            {
                if (info_bits[i])
                    m_phase += pi2 / 2.;
                for (uint64_t j = 0; j < info_bits_period; ++j)
                {
                    out_data[j + i * info_bits_period] = sin(m_phase) * AMPL * m_volume;
                    m_phase += d_phase;
                    if (m_phase >= pi2)
                        m_phase -= pi2;
                }
            }
        }
        else
        {
            for(uint64_t i = 0; i < info_bits.size(); i += 2)
            {
                int64_t I = (info_bits[i] ? 1 : -1);
                int64_t Q = (info_bits[i + 1] ? 1 : -1);
                //std::cout << "I: " << I << ", Q: " << Q << std::endl;
                // I component
                for (uint64_t j = 0; j < info_bits_period * 2; ++j)
                {
                    out_data[j + i * info_bits_period] = sin(m_phase) * AMPL * m_volume * I;
                    out_data[j + i * info_bits_period] += sin(m_phase + pi2 / 4) * AMPL * m_volume * Q;
                    m_phase += d_phase;
                    if (m_phase >= pi2)
                        m_phase -= pi2;
                }
            }

        }
        return;
    }

    // Set mod type
    void SetModType(bool type)
    {
        isFM2 = type;

        return;
    }
};

#endif // _GENERATOR_H_