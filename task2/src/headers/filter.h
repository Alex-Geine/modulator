#ifndef _FILTER_H_
#define _FILTER_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include "math_func.h"
#include "file_writer.h"

#define AMPL_SIG 0x7FFF
typedef std::vector<std::complex<double>> cmplx;

template<typename DATA_TYPE>
struct FilterDataSample
{
    std::vector<DATA_TYPE>  m_temp_data;          // temp data will be filled by signal data at previous step
    std::vector<DATA_TYPE>* m_signal_data;        // pointer to signal data
    uint32_t                m_temp_data_size = 0; // temp data size
    uint32_t m_signal_data_size = 0;

    FilterDataSample(uint32_t temp_data_size = 10): m_temp_data_size(temp_data_size)
    {
        m_temp_data.resize(m_temp_data_size);
    }

    // Get pointer to signal data sample
    void getSignalData(std::vector<DATA_TYPE>* signal_data)
    {
        m_signal_data = signal_data;
        m_signal_data_size = signal_data->size();
    }

    // Set temp data from signal data sample
    void setTempData()
    {
        if (m_signal_data == nullptr)
            return;

        // filling temp_data
        for (uint32_t i = 0; i < m_temp_data_size; ++i)
            m_temp_data[i] = (*m_signal_data)[m_signal_data_size + i - m_temp_data_size];

        return;
    }

    DATA_TYPE operator[](uint32_t i)
    {
        if (i < m_temp_data_size)
        {
            //std::cout << "i: " << i << ", first" << std::endl;
            return m_temp_data[i];
        }
        else if (i < m_temp_data_size + m_signal_data_size)
        {
            //std::cout << "i: " << i << ", second" << std::endl;
            return (*m_signal_data)[i - m_temp_data_size];
        }
        else if (i < 2 * m_temp_data_size + m_signal_data_size)
        {
            //std::cout << "i: " << i << ", third" << std::endl;
            return m_temp_data[i - m_signal_data_size - m_temp_data_size];
        }
        else
        {
            //std::cout << "i: " << i << ", zero" << std::endl;
            return 0;
        }

        return 0;
    }
};


// Class of filter model
template<typename DATA_TYPE>
class Filter
{
    private:

    double                 m_cut_freq    = 0;  // Cutoff frequency of filter
    double                 m_sample_rate = 0;  // Sample rate of input signal
    uint32_t               m_filter_size = 0;  // Size of filter window
    std::vector<DATA_TYPE> m_filter_data;      // Array with filter data
    FilterDataSample<DATA_TYPE> m_filter_data_sample;

    private:

    // 2 f0 sin(2 Pi f0 t) / (2 Pi f0 t), f0 - cutoff freq
    DATA_TYPE sinc(double t)
    {
        if (t != 0)
            return sin(M_PI * 2 * m_cut_freq * t) / M_PI / t * AMPL_SIG;

        return AMPL_SIG;
    };

    public:

    void convolution(std::vector<DATA_TYPE>& data_out)
    {
        for (uint32_t i = 0; i < data_out.size(); ++i)
        {
            std::cout << "i: " << i << std::endl;
            for (uint32_t j = 0; j < m_filter_size; ++j)
            {
                std::cout << "j: " << j << std::endl;
                data_out[i] += m_filter_data_sample[i + j] * m_filter_data[j];
                std::cout << "data: " << m_filter_data_sample[i+j] << ", filt: " << m_filter_data[j] << ", res: " << data_out[i] << std::endl;
            }
            data_out[i] /= m_filter_size;
        }

        return;
    }

    void gen_filter(std::vector<DATA_TYPE>& filter_data)
    {
        filter_data.clear();
        filter_data.resize(m_filter_size);

        //std::cout << "m_sample_rate: " << m_sample_rate << ", m_filter_size: " << m_filter_size << std::endl;
        double T = 1 / m_sample_rate;
        double t = - (double)m_filter_size / 2. * T;

        for (uint32_t i = 0; i < m_filter_size; ++i)
        {
            //std::cout << "t: " << t << std::endl;
            filter_data[i] = sinc(t);
            //std::cout << filter_data[i] << " ";
            t += T;
        }

        //std::cout << std::endl;

        return;
    }

    // Constructor
    /*
        [in] cut_freq    - cutoff frequency of filter
        [in] sample_rate - sample rate of signal
    */
    Filter(double cut_freq, double sample_rate, uint32_t filter_size) : m_cut_freq(cut_freq), m_sample_rate(sample_rate), m_filter_size(filter_size)
    {
        gen_filter(m_filter_data);
    };

    void Process(std::vector<DATA_TYPE>& data_in, std::vector<DATA_TYPE>& data_out)
    {
        data_out.clear();
        data_out.resize(data_in.size());
        // create sinc low-pass filter impulse characteristic
        m_filter_data_sample.getSignalData(&data_in);

        convolution(data_out);

        m_filter_data_sample.setTempData();

        return;
    }
};

#endif // _FILTER_H_
