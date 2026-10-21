/*
 * A276853  Beatty sequence for 2*e.
 * https://oeis.org/A276853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*e*n)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 20! = 2432902008176640000ULL */
    static uint64_t fact[21] = {0};
    static unsigned __int128 denom = 0;
    static unsigned __int128 T = 0;
    if (denom == 0) {
        fact[0] = 1ULL;
        for (int i = 1; i <= 20; ++i)
            fact[i] = fact[i - 1] * (uint64_t)i;      /* 20! fits in 64‑bit */

        denom = fact[20];   /* 20! */
        unsigned __int128 sum = 0;
        for (int k = 0; k <= 20; ++k)
            sum += (unsigned __int128)denom / fact[k];
        T = sum;
    }

    /* compute floor((2 * n * T) / denom) */
    unsigned __int128 prod = (unsigned __int128)2 * (unsigned __int128)n * T;
    unsigned __int128 res  = prod / denom;
    return (seq_t)res;
}
