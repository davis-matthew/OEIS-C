/*
 * A327213  Positions of 1's in {A327211(n) : n > 0}.
 * https://oeis.org/A327213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327211(k) = 1, where A327211(k) =
 *            floor((2k+2)e) - floor((k+2)e) - floor(k*e); found by linear
 *            search
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9451 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A327213_fl(unsigned long long k)
{
    const unsigned __int128 E = (unsigned __int128)2718281828459045235ULL; /* floor(e*10^18) */
    const unsigned __int128 S = (unsigned __int128)1000000000000000000ULL;
    return (unsigned long long)(((unsigned __int128)k * E) / S);
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    unsigned long long k = 0;
    while (cnt < n) {
        ++k;
        long long v = (long long)A327213_fl(2 * k + 2)
                    - (long long)A327213_fl(k + 2)
                    - (long long)A327213_fl(k);
        if (v == 1)
            ++cnt;
    }
    return (seq_t)k;
}
