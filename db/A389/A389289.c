/*
 * A389289  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *          binomial(k,n-2*k).
 * https://oeis.org/A389289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *            binomial(k,n-2*k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"  /* Provides seq_t binomial(idx_t, idx_t); */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 directly (otherwise binomial(-1,0) would be called). */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t max_k = n >> 1;          /* floor(n/2) */

    /* k = 0 contributes only when n = 0, so start at 1. */
    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t j = n - (k << 1);    /* n - 2*k */

        /* When j > k, binomial(k, j) is 0 (outside the range of k). */
        if (j > k)
            continue;

        seq_t term1 = binomial(n + k - 1, k);
        seq_t term2 = binomial(k, j);
        sum += term1 * term2;
    }

    return sum;
}
