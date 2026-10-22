/*
 * A389348  Expansion of (1/x) * Series_Reversion( x * (1 - x^3 / (1 - x)^2)
 *          ).
 * https://oeis.org/A389348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(n+k,k) *
 *            binomial(n-k-1,n-3k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 86,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* a(0) = 1 */

    /* For 64‑bit or smaller seq_t we use 128‑bit intermediates
       to avoid overflow; for larger seq_t we rely on seq_mul/big‐int. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        __uint128_t sum = 0;
        idx_t kmax = n / 3;
        for (idx_t k = 0; k <= kmax; ++k)
        {
            seq_t b1 = binomial(n + k, k);
            seq_t b2 = binomial(n - k - 1, n - 3 * k);
            sum += (__uint128_t)b1 * (__uint128_t)b2;
        }
        return (seq_t)(sum / ((seq_t)n + 1));
    }
    else
    {
        seq_t sum = 0;
        idx_t kmax = n / 3;
        for (idx_t k = 0; k <= kmax; ++k)
        {
            seq_t b1 = binomial(n + k, k);
            seq_t b2 = binomial(n - k - 1, n - 3 * k);
            sum += seq_mul(b1, b2);
        }
        return sum / ((seq_t)n + 1);
    }
}
