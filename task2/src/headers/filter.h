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

    FilterDataSample(uint32_t temp_data_size): m_temp_data_size(temp_data_size)
    {
        m_temp_data.resize(m_temp_data_size);
    }

    // Get pointer to signal data sample
    void getSignalData(std::vector<DATA_TYPE>* signal_data)
    {
        m_signal_data = signal_data;
    }

    // Set temp data from signal data sample
    void setTempData()
    {
        if (m_signal_data == nullptr)
            return;

        uint32_t signal_data_size = m_signal_data->size();

        // filling temp_data
        for ()
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
    std::vector<DATA_TYPE> m_signal_data;      // Array with part of signal data

    private:

    // 2 f0 sin(2 Pi f0 t) / (2 Pi f0 t), f0 - cutoff freq
    DATA_TYPE sinc(double t)
    {
        if (t != 0)
            return sin(M_PI * 2 * m_cut_freq * t) / M_PI / t * AMPL_SIG;

        return AMPL_SIG;
    };

    public:

    void convolution(const std::vector<DATA_TYPE>& data_signal, const std::vector<DATA_TYPE>& data_filter, std::vector<DATA_TYPE>& out_data)
    {
        uint32_t size = data_signal.size();
        int32_t id_filter = 0;

        for (uint32_t i = 0; i < size; ++i)
        {
            for (uint32_t j = 0; j <= i; ++j)
            {
                if (size - 1 - j >= 0)
                    out_data[i] += data_signal[j] * data_filter[size - 1 - j];
            }
        }

        return;
    }

    void gen_filter(std::vector<DATA_TYPE>& filter_data)
    {
        filter_data.clear();
        filter_data.resize(m_filter_size);

        double T = 1 / m_sample_rate;
        double t = 0;

        for (uint32_t i = 0; i < m_filter_size; ++i)
        {
            filter_data[i] = sinc(t);
            std::cout << filter_data[i] << " ";
            t += T;
        }
        std::cout << std::endl;

        return;
    }

    // Constructor
    /*
        [in] cut_freq    - cutoff frequency of filter
        [in] sample_rate - sample rate of signal
    */
    Filter(double cut_freq, double sample_rate, uint32_t filter_size) : m_cut_freq(cut_freq), m_sample_rate(sample_rate), m_filter_size(filter_size){};

    void Process(const std::vector<DATA_TYPE>& data_in, std::vector<DATA_TYPE>& data_out)
    {
        // create sinc low-pass filter impulse characteristic
        uint32_t size = data_in.size();

        // create sinc low-pass filter impulse characteristic
        //convolution(data_in, filter_data, data_out);

        return;
    }
};

#endif // _FILTER_H_
