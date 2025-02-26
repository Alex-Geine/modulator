#ifndef _FILE_WRITER_H_
#define _FILE_WRITER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// function for writing data in file
/*
    [in] x_data    - array with x axis data
    [in] y_data    - array with y axis data
    [in] file_name - name of file to write
*/
template <typename X_DATA_TYPE, typename Y_DATA_TYPE>
void writeToFile(const std::vector<X_DATA_TYPE>& x_data, const std::vector<Y_DATA_TYPE>& y_data, const std::string& file_name)
{
    std::ofstream file(file_name);

    for (uint64_t i = 0; i < x_data.size(); ++i)
        file << x_data[i] << " " << y_data[i] << "\n";

    file.close();

    return;
}

#endif // _FILE_WRITER_H