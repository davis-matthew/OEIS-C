/*
 * A327212  Positions of 0's in {A327211(n) : n > 0}.
 * https://oeis.org/A327212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327211(k) = floor((2k+2)e) -
 *            floor((k+2)e) - floor(k e) = 0; found by linear scan
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9668 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long A327212_fl(unsigned long long k)
{
    /* floor(k*e) using e ~ 2.718281828459045235 (scale 1e18), exact for the range needed */
    unsigned __int128 p = (unsigned __int128)k * (unsigned __int128)2718281828459045235ULL;
    return (long long)(p / (unsigned __int128)1000000000000000000ULL);
}

seq_t a(idx_t n)
{
    unsigned long long cnt = 0;
    unsigned long long k = 0;
    while (cnt < (unsigned long long)n) {
        ++k;
        long long v = A327212_fl(2 * k + 2) - A327212_fl(k + 2) - A327212_fl(k);
        if (v == 0)
            ++cnt;
    }
    return (seq_t)k;
}
