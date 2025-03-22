#include "file_writer.h"
#include "receiver.h"
#include "src/headers/file_writer.h"
#include "src/headers/filter.h"
#include "src/headers/generator.h"
#include "src/headers/modulator.h"

void g_error_processor(const std::vector<int64_t>& data_1,
                       const std::vector<int64_t>& data_2,
                             std::vector<double>&   error_data)
{
    error_data.clear();
    error_data.resize(data_1.size());

    for (uint64_t i = 0; i < data_1.size(); ++i)
        error_data[i] = ((double)data_1[i] - (double)data_2[i]) / AMPL / AMPL;

    return;
}

void g_error_processorQPSK(
    const std::vector<int64_t>& data_1,
    const std::vector<int64_t>& data_2,
          std::vector<double>&   error_data)
{
error_data.clear();
error_data.resize(data_1.size());

for (uint64_t i = 0; i < data_1.size(); ++i)
error_data[i] = ((double)data_1[i] * (double)data_2[i]) / AMPL / AMPL / AMPL / AMPL;

return;
}

int main()
{
    std::vector<int64_t> data;          // Data from generator
    std::vector<int64_t> receive_data_1;// Data from channel 1 from receiver
    std::vector<int64_t> receive_data_2;// Data from channel 2 from receiver
    std::vector<double>  time_p;        // Time points data

    double   freq        = 110;         // Frequency of signal
    double   sample_rate = 1000;        // Sample rate of signal
    uint64_t N           = 1000;        // Number of points
    uint64_t filter_size = 128;         // Size of window filter
    double   phase       = 0;           // Phase of generated signal in 
    double   max_phase   = pi2;         // Maximum phase to research
    int      num_steps   = 100;         // Number of points to research
    double   d_phase     = max_phase / num_steps;
    bool isFM2 = false;

    data.resize(N);
    time_p.resize(N);

    for(uint64_t i = 0; i < N; ++i)
        time_p[i] = i;

    Modulator mod(freq, sample_rate);
    Receiver receiver(freq, sample_rate, filter_size);
    //Filter<int64_t> filter(1.5 * freq, sample_rate, filter_size);


    std::vector<uint8_t> info_bits{1,0,1,0,1,0,1,0,1,0};
    mod.GenData(info_bits, data);
    mod.SetModType(isFM2);

    //std::cout << "dataSize: " << data.size() << std::endl;
    //for (auto &it: data)
    //    std::cout << it << " ";
    //
    // std::cout << std::endl;
    writeToFile(time_p, data, "gen_data.txt");
    /*
    FilterDataSample<int64_t> sample;
    sample.getSignalData(&data);

    for (uint64_t i =0; i < N; ++i)
        std::cout << data[i] << ' ';
    std::cout << std::endl;

    writeToFile(time_p, data, "receive_data_2.txt");

    std::cout << "Filter Data Sample Test: tempDataSize: " << sample.m_temp_data_size << " , signal size: " << sample.m_signal_data_size << std::endl;
    uint64_t sample_size = 2 * sample.m_temp_data_size + sample.m_signal_data_size;
    for (uint64_t i = 0; i < sample_size; ++i)
        std::cout << sample[i] << " ";
    std::cout << std::endl;

    */

    //std::vector<int64_t> filter_data;
    // std::vector<int64_t> filter_ids(filter_size);
    // std::vector<int64_t> out_data_filter(N);
    // std::vector<std::complex<double>> data_spec(N);
    // std::vector<std::complex<double>> data_out_spec(N);
    // std::vector<double> data_spec_d(N);
    // std::vector<double> data_out_spec_d(N);
    // std::vector<std::complex<double>> data_sinc_spec(filter_size);
    // std::vector<double> data_sinc_spec_d(filter_size);

    // for(uint64_t i = 0; i < filter_size; ++i)
    //     filter_ids[i] = i;

    // filter.gen_filter(filter_data);
    // filter.Process(data, out_data_filter);

    // writeToFile(time_p, out_data_filter, "spectre_filt.txt");
/*
    for (uint64_t i =0; i < N; ++i)
    {
        data_spec[i].real((double)data[i] / AMPL_SIG);
        data_out_spec[i].real((double)out_data_filter[i] / AMPL_SIG);
    }

    for (uint64_t i = 0; i < filter_size; ++i)
        data_sinc_spec[i].real((double)filter_data[i] / AMPL_SIG);

    g_fft(data_spec, DIRECT);
    g_fft(data_out_spec, DIRECT);
    g_fft(data_sinc_spec, DIRECT);

    for (uint64_t i =0; i < N; ++i)
    {
        data_spec_d[i] = std::abs(data_spec[i]);
        data_out_spec_d[i] = std::abs(data_out_spec[i]);
    }

    for (uint64_t i = 0; i < filter_size; ++i)
        data_sinc_spec_d[i] = std::abs(data_sinc_spec[i]);

    writeToFile(time_p, data_spec_d, "receive_data_1.txt");
    writeToFile(time_p, data_out_spec_d, "receive_data_2.txt");
    writeToFile(filter_ids, data_sinc_spec_d, "spectre.txt");
*/
    
    std::vector<double> error_data;
    std::vector<double> error_ids(num_steps);
    std::vector<double> avg_error(num_steps, 0);

    for (int i = 0; i < num_steps; ++i)
    {
        error_ids[i] = phase;
        // Generate data
        mod.SetPhase(phase);
        mod.GenData(info_bits, data);

        // Receive data
        receiver.Process(data, receive_data_1, receive_data_2, !isFM2);

        g_error_processorQPSK(receive_data_1, receive_data_2, error_data);

        for(auto&item:error_data)
            avg_error[i] += item;
        avg_error[i] /= error_data.size();

        phase += d_phase;
       /* if (fabs(avg_error[i]) < 0.1)
            std::cout << "Zero error phase: " << phase << ", error: " << avg_error[i] << ", i: " << i << std::endl;
            */
    }

    writeToFile(time_p, receive_data_1, "receive_data_1.txt");
    writeToFile(time_p, receive_data_2, "receive_data_2.txt");
    writeToFile(error_ids, avg_error, "error_data.txt");
    
    return 0;
}
