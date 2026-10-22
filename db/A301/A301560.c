/*
 * A301560  Matching number of the n-odd graph.
 * https://oeis.org/A301560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(binomial(2*n-1, n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, so a(0) is unused but safe */

    /* Compute binomial(2*n-1, n-1) */
    idx_t N = 2*n - (idx_t)1;
    idx_t K = n - (idx_t)1;

    seq_t v = binomial(N, K);

    /* Floor division by 2 (unsigned truncation) */
    return v / (seq_t)2;
}
