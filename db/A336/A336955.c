/*
 * A336955  a(n) = Sum_{k=0..n} k^k * binomial(n, k)^2.
 * https://oeis.org/A336955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^k * binomial(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;           /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t kpow;
        if (k == 0)             /* 0^0 is defined as 1 */
            kpow = 1;
        else
            kpow = seq_pow((seq_t)k, k);

        seq_t term = kpow * binom * binom;
        sum += term;

        if (k < n)              /* prepare binomial(n,k+1) */
            binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
    }

    return sum;
}
