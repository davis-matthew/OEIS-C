/*
 * A349969  a(n) = Sum_{k=0..n} (k*n)^(n-k).
 * https://oeis.org/A349969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k*n)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t exp = n - k;
        seq_t term;

        /* (k*n)^0 = 1 for any k; 0^exp = 0 if exp > 0 */
        if (exp == 0)
            term = 1;
        else if (k == 0)
            term = 0;
        else
        {
            /* base = k * n, all in seq_t to avoid overflow on 64‑bit builds */
            seq_t base = (seq_t)k * (seq_t)n;
            term = big_pow(base, (seq_t)exp);
        }

        sum += term;
    }
    return sum;
}
