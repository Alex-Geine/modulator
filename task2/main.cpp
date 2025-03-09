#include "file_writer.h"
#include "receiver.h"
#include "src/headers/file_writer.h"
#include "src/headers/generator.h"

void g_error_processor(const std::vector<int32_t>& data_1,
                       const std::vector<int32_t>& data_2,
                             std::vector<double>&   error_data)
{
    error_data.clear();
    error_data.resize(data_1.size());

    for (uint64_t i = 0; i < data_1.size(); ++i)
        error_data[i] = ((double)data_1[i] - (double)data_2[i]) / AMPL / AMPL;

    return;
}

void g_error_processorQPSK(
    const std::vector<int32_t>& data_1,
    const std::vector<int32_t>& data_2,
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
    std::vector<int16_t> data;          // Data from generator
    std::vector<int16_t> filter_data;   // Filtered data from generator
    std::vector<int32_t> receive_data_1;// Data from channel 1 from receiver
    std::vector<int32_t> receive_data_2;// Data from channel 2 from receiver
    std::vector<double>  time_p;        // Time points data

    double   freq        = 110;         // Frequency of signal
    double   sample_rate = 1000;        // Sample rate of signal
    uint64_t N           = 1000;        // Number of points
    double   phase       = 0;           // Phase of generated signal in 
    double   max_phase   = pi2;
    int      num_steps = 100;
    double   d_phase = max_phase / num_steps;

    data.resize(N);
    filter_data.resize(N);
    time_p.resize(N);
    
    for(uint64_t i = 0; i < N; ++i)
        time_p[i] = i;


    Generator gen(freq, sample_rate);
    Receiver receiver(freq, sample_rate);

    std::vector<double> error_data;
    std::vector<double> error_ids(num_steps);
    std::vector<double> avg_error(num_steps, 0);

    for (int i = 0; i < num_steps; ++i)
    {
        error_ids[i] = phase;
        // Generate data
        gen.SetPhase(phase);
        gen.GenData(data);

        // Receive data
        receiver.Process(data, receive_data_1, receive_data_2, true);

        g_error_processor(receive_data_1, receive_data_2, error_data);

        for(auto&item:error_data)
            avg_error[i] += item;
        avg_error[i] /= error_data.size();

        phase += d_phase;
       /* if (fabs(avg_error[i]) < 0.1)
            std::cout << "Zero error phase: " << phase << ", error: " << avg_error[i] << ", i: " << i << std::endl;
    */}

    writeToFile(error_ids, avg_error, "error_data.txt");

    return 0;
}