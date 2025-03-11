#ifndef _FILTER_H_
#define _FILTER_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include "math_func.h"
#include "file_writer.h"

typedef std::vector<std::complex<double>> cmplx;

// Class of filter model
class Filter
{
    private:

    double m_cut_freq    = 0;    // Cutoff frequency of filter
    double m_sample_rate = 0;    // Sample rate of input signal

    private:

    // ConvertIn - convert input data fucntion
    /*
        [in]  data_in  - input data to convert
        [out] data_out - output complex data
        return void
    */
    template<typename DATA_TYPE>
    void ConvertIn(const std::vector<DATA_TYPE>& data_in,  cmplx& data_out)
    {
        uint64_t size     = data_in.size();
        uint64_t size_out = 2;

        // Find size which is multyply of two
        while (size_out <= size)
            size_out = size_out << 1;

        // If data_in size is not multyply of two, size was changed and other data will be zeroes.
        data_out.clear();
        data_out.resize(size_out);
    
        for (uint64_t i = 0; i < size; ++i)
            data_out[i].real(static_cast<double>(data_in[i]));
 
        return;
    }

    // 2f0sin(2Pif0t)/(2Pif0t), f0 - cutoff freq
    double sinc(double t)
    {
        if (t != 0)
            return 2 * sin(M_PI * 2 * m_cut_freq * t) / M_PI / 2 / t;
        
        return 1;
    };





    // ConvertOut - convert input data fucntion
    /*
        [in]  data_in  - input complex data to convert
        [out] data_out - output data
        return void
    */
    template<typename DATA_TYPE>
    void ConvertOut(const cmplx& data_in, std::vector<DATA_TYPE>& data_out)
    {
        uint64_t size = data_out.size();
 
        for (uint64_t i = 0; i < size; ++i)
            data_out[i] = static_cast<DATA_TYPE>(data_in[i].real());

        return;
    }

    public:

    template<typename DATA_TYPE>
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

    template<typename DATA_TYPE>
    void gen_filter(std::vector<DATA_TYPE>& filter_data)
    {
        uint32_t size = filter_data.size();

        double T = 1 / m_sample_rate;
        double t = 0;

        for (uint32_t i = 0; i < size; ++i)
        {
            filter_data[i] = sinc(t);
            t += T;
        }

        return;
    }

    // Constructor
    /*
        [in] cut_freq    - cutoff frequency of filter
        [in] sample_rate - sample rate of signal
    */
    Filter(double cut_freq, double sample_rate) : m_cut_freq(cut_freq), m_sample_rate(sample_rate){};

    // Process - function for filtering data
    /*
        [in]  data_in  - array of input data to filtering
        [out] data_out - array of filtered data
        return void        
    */
    template<typename DATA_TYPE>
    void Process(const std::vector<DATA_TYPE>& data_in, std::vector<DATA_TYPE>& data_out)
    {
        data_out.clear();
        data_out.resize(data_in.size());

        cmplx temp_data;

        // We need to convert data to complex<double> and make data size multiply of two
        ConvertIn(data_in, temp_data);

        // Use FFT on temp data
        if (!g_fft(temp_data, DIRECT))
        {
            std::cout << "FFT is failed!!" << std::endl;
            return;
        }

        // We need to calculate, which points in spectrum will be cutted
        /*
            we must kmow how spenctum was sapled
            d_f - sample step for frequency
            d_f = sample_rate / number_of_points
        */
        uint64_t start_id = 0;
        double d_f = m_sample_rate / temp_data.size();

        start_id = m_cut_freq / d_f;

        //std::cout << "d_f: " << d_f << std::endl;
        //std::cout << "start_id: " << start_id << std::endl;

        // output spectre
        std::vector<double> ampl;
        std::vector<double> x;
        ampl.resize(temp_data.size());
        x.resize(temp_data.size());

        for (uint64_t i = 0; i < temp_data.size(); ++i)
        {
            x[i] = i;
            ampl[i] = std::abs(temp_data[i]);
        }

        writeToFile(x, ampl, "spectre.txt");

        // Filtering spectrum
        for (uint64_t i = start_id; i < temp_data.size(); ++i)
            temp_data[i] = {0., 0.};

        for (uint64_t i = 0; i < temp_data.size(); ++i)
        {
            x[i] = i;
            ampl[i] = std::abs(temp_data[i]);
        }
    
        writeToFile(x, ampl, "spectre_filt.txt");

        // Use FFT^-1 on filtered spectrum
        if (!g_fft(temp_data, INVERSE))
        {
            std::cout << "Inverse FFT was failed!!" << std::endl;
            return;
        }

        // Convert data from complex double to initial type
        ConvertOut(temp_data, data_out);
        
        return;
    }
    
    template<typename DATA_TYPE>
    void Process_v2(const std::vector<DATA_TYPE>& data_in, std::vector<DATA_TYPE>& data_out)
    {
        // create sinc low-pass filter impulse characteristic
        uint32_t size = data_in.size();
        std::vector<DATA_TYPE> filter_data(size);

        // create sinc low-pass filter impulse characteristic
        convolution(data_in, filter_data, data_out);

        return;
    }
};

#endif // _FILTER_H_
