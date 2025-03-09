#ifndef _MATH_FUNC_H_
#define _MATH_FUNC_H_

#include <complex>
#include <vector>
#include <cstdint>

#define DIRECT  true
#define INVERSE false

// FFT - Fast Furie Transform function
/*
    [in/out] data - data for convertation
    [in]     flag - direction of transform (true - direct, false - reverce)
    return false if error, true - otherwise
*/
bool g_fft(std::vector<std::complex<double>>& data, bool flag);

#endif // _MATH_FUNC_H_