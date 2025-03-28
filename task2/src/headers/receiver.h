#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "filter.h"
#include "generator.h"

// Class for receiver model
class Receiver
{
    private:

    Filter<int64_t>*  m_filter      = nullptr;    // Filter model
    Generator*        m_generator   = nullptr;    // Generator model
    double            m_freq        = 0;          // Signal frequency
    double            m_sample_rate = 0;          // Sample rate
    double            m_d_phase     = 0;          // phase correction signal
    double            m_koeff       = 0.1;        // error phasr koeff


    template<typename T> T Sign(T val){
        T res = (val >= 0) ? 1: -1;
        //if (res == -1)
          //  std::cout << val << std::endl;
        return res;
    }

    public:

    // Constructor
    /*
        [in] freq        - frequency of signal
        [in] sample_rate - sample rate
    */
    Receiver(double freq, double sample_rate, uint32_t filter_size) : m_freq(freq), m_sample_rate(sample_rate)
    {
        m_filter    = new Filter<int64_t>(1.5 * m_freq, m_sample_rate, filter_size);
        m_generator = new Generator(m_freq, m_sample_rate);
    }

    // Destructor
    ~Receiver()
    {
        if (m_filter != nullptr)
            delete m_filter;
        if (m_generator != nullptr)
            delete m_generator;
    }

    // Process - fucntion for receive data
    /*
        [in]  data_in    - input data array
        [out] data_out_1 - output data from channel one (sin)
        [out] data_out_1 - output data from channel teo (cos)
        return void
    */
    template<typename DATA_TYPE, typename OUT_DATA_TYPE>
    void Process(const std::vector<DATA_TYPE>& data_in, std::vector<OUT_DATA_TYPE>& data_out_1, std::vector<OUT_DATA_TYPE>& data_out_2, double& error, bool isQPSK = false)
    {
        uint64_t size = data_in.size();

        data_out_1.clear();
        data_out_2.clear();

        data_out_1.resize(size);
        data_out_2.resize(size);

        // Generate signals (sin/cos) to multyply with input data
        std::vector<DATA_TYPE> temp_data(size);

        // Generate sin data
        m_generator->SetPhase(0. + m_d_phase);
        m_generator->GenData(temp_data);

        // Multyply sin
        for (uint64_t i = 0; i < size; ++i)
            temp_data[i] *= data_in[i];

        // Filtering sin data
        m_filter->Process(temp_data, data_out_1);

        // Generate cos data
        m_generator->SetPhase(pi2 / 4.);
        m_generator->GenData(temp_data);

        // Multyply cos
        for (uint64_t i = 0; i < size; ++i)
            temp_data[i] *= data_in[i];

        // Filtering cos data
        m_filter->Process(temp_data, data_out_2);

        // std::vector<int> time_p(size);
        // for (auto i = 0; i < size; ++i)
            // time_p[i] = i;


        // Its error detecting fucntion
        // QPSK receiver processing

        
        if (isQPSK)
        {
            OUT_DATA_TYPE error_phase;
            for (uint64_t i = 0; i < data_out_1.size(); ++i){

                error_phase +=  data_out_1[i] * Sign<OUT_DATA_TYPE>(data_out_2[i]) - data_out_2[i] * Sign<OUT_DATA_TYPE>(data_out_1[i]);
                //std::cout << "error_phase: " << error_phase << std::endl;
            }
            std::cout << "first: " << data_out_1[0] * Sign<OUT_DATA_TYPE>(data_out_2[0]) << ", second: " << data_out_2[0] * Sign<OUT_DATA_TYPE>(data_out_1[0]) << std::endl;

            m_d_phase = (double)error_phase / data_out_1.size() / AMPL / AMPL * m_koeff;

            std::cout << "m_phase: " << m_d_phase << ", error_phase: " << error_phase << ", data_size: " << data_out_1.size() << ", ampl: " << AMPL << ", koef: " << m_koeff << std::endl;

            error = m_d_phase;
        }
        
        
        return;
    }
};

#endif // _RECEIVER_H_