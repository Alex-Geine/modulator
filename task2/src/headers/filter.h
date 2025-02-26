#ifndef _FILTER_H_
#define _FILTER_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include "math_func.h"

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

        // Filtering spectrum
        for (uint64_t i = start_id; i < temp_data.size(); ++i)
            temp_data[i] = {0., 0.};

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
};

#endif // _FILTER_H_
