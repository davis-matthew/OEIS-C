/*
 * A273652  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, two of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273652
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 * (1 + 2^(n-2))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(n,2) */
    seq_t comb = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Compute 2^(n-2) by repeated doubling */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 = pow2 * (seq_t)2;

    /* Sum 1 + 2^(n-2) */
    seq_t sum = pow2 + (seq_t)1;

    /* Final result */
    return comb * sum;
}
