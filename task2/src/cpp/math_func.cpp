#include "math_func.h"

#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...

// FFT - Fast Furie Transform function
/*
    [in/out] data - data for convertation
    [in]     flag - direction of transform (true - direct, false - reverce)
*/
bool g_fft(std::vector<std::complex<double>>& data, bool flag)
{
    // Prepare data
    uint64_t logN = std::log2(data.size());
    uint64_t size = data.size();

    // Parameters error check:
    if ((size > 16384) || (size < 1))                      return false;
    if (!NUMBER_IS_2_POW_K(size))                          return false;
    if ((logN < 2) || (logN > 14))                         return false;
    
    uint64_t  i, j, n, k, io, ie, in, nn;
    double    ru, iu, rtp, itp, rtq, itq, rw, iw, sr;
    
    static const double Rcoef[14] =
    {  -1.0000000000000000D,  0.0000000000000000D,  0.7071067811865475D,
        0.9238795325112867D,  0.9807852804032304D,  0.9951847266721969D,
        0.9987954562051724D,  0.9996988186962042D,  0.9999247018391445D,
        0.9999811752826011D,  0.9999952938095761D,  0.9999988234517018D,
        0.9999997058628822D,  0.9999999264657178D
    };
    static const double Icoef[14] =
    {   0.0000000000000000D, -1.0000000000000000D, -0.7071067811865474D,
       -0.3826834323650897D, -0.1950903220161282D, -0.0980171403295606D,
       -0.0490676743274180D, -0.0245412285229122D, -0.0122715382857199D,
       -0.0061358846491544D, -0.0030679567629659D, -0.0015339801862847D,
       -0.0007669903187427D, -0.0003834951875714D
    };
    
    nn = size >> 1;
    ie = size;
    
    for (n = 1; n <= logN; ++n)
    {
        rw = Rcoef[logN - n];
        iw = Icoef[logN - n];
    
        if (flag == INVERSE)
            iw = -iw;
    
        in = ie >> 1;
        ru = 1.0D;
        iu = 0.0D;
    
        for (j = 0; j < in; ++j)
        {
            for (i = j; i < size; i += ie)
            {
                io  = i + in;
                rtp = data[i].real() + data[io].real();
                itp = data[i].imag() + data[io].imag();
                rtq = data[i].real() - data[io].real();
                itq = data[i].imag() - data[io].imag();

                data[io].real(rtq * ru - itq * iu);
                data[io].imag(itq * ru + rtq * iu);
                data[i].real(rtp);
                data[i].imag(itp);
            }

            sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
    
        ie >>= 1;
    }
    
    for (j = i = 1; i < size; ++i)
    {
        if (i < j)
        {
            io  = i - 1;
            in  = j - 1;
            rtp = data[in].real();
            itp = data[in].imag();

            data[in] = data[io];
            data[io].real(rtp);
            data[io].imag(itp);
        }
    
        k = nn;
    
        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }
    
        j = j + k;
    }
    
    if (flag == DIRECT)
        return true;
    
    rw = 1.0D / size;
    
    for (i = 0; i < size; ++i)
        data[i] *= rw;
    
    return true;
}