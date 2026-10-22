/*
 * A389440  Expansion of (1/x) * Series_Reversion( x * (1 - x^3 * (1 + x)^2) /
 *          (1 + x) ).
 * https://oeis.org/A389440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binom(n+k,k) *
 *            binom(n+2k+1,n-3k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For small integer seq_t (≤ 64 bits) we use 128‑bit intermediates
       to avoid overflow of the product and the running sum. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        unsigned __int128 sum = 0;
        idx_t maxk = n / 3;
        for (idx_t k = 0; k <= maxk; ++k)
        {
            seq_t t1 = binomial(n + k, k);
            seq_t t2 = binomial(n + 2 * k + 1, n - 3 * k);
            unsigned __int128 prod = (unsigned __int128)t1 * (unsigned __int128)t2;
            sum += prod;
        }
        return (seq_t)(sum / (unsigned __int128)(n + 1));
    }

    /* For arbitrary‑precision seq_t we can rely on the built‑in
       arithmetic of the class/typedef. */
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n + k, k);
        seq_t t2 = binomial(n + 2 * k + 1, n - 3 * k);
        sum += t1 * t2;
    }
    return sum / (seq_t)(n + 1);
}
