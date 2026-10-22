/*
 * A369192  Number of labeled simple graphs with n vertices and at most n
 *          edges (not necessarily covering).
 * https://oeis.org/A369192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n*(n-1)/2, k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute M = n*(n-1)/2 as seq_t, taking care of underflow when n < 2 */
    seq_t M;
    if (n < 2)
        M = 0;
    else
        M = ((seq_t)n * ((seq_t)n - 1)) / 2;

    seq_t sum = 0;
    seq_t comb = 1;          /* C(M,0) */

    sum += comb;             /* k = 0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update combinatorial value */
        comb = (comb * (M + 1 - (seq_t)k)) / (seq_t)k;
        sum += comb;
    }

    return sum;
}
