/*
 * A382496  a(n) = Sum_{k=0..floor(n/3)} (k+1) * binomial(2*k,2*n-6*k).
 * https://oeis.org/A382496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (k+1) * binomial(2*k, 2*n-6*k)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 216, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* k ranges from 0 to floor(n/3) */
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        idx_t kk  = 2 * k;
        idx_t kk2 = 2 * (n - 3 * k);   /* 2*n - 6*k  */
        if (kk2 > kk) /* binomial(kk, kk2) is zero for kk2 > kk */
            continue;
        seq_t b   = binomial(kk, kk2);
        seq_t term = (seq_t)(k + 1) * b;
        sum += term;
    }
    return sum;
}
