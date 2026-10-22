/*
 * A398914  a(n) = n * (n + 2) * binomial(2*n + 1, n + 1).
 * https://oeis.org/A398914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n + 2) * binomial(2*n + 1, n + 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute the multiplicative factor n*(n+2) in seq_t */
    seq_t factor = (seq_t)n * ((seq_t)n + 2);

    /* binomial(2*n + 1, n + 1) returns a seq_t value */
    seq_t bin = binomial(2 * n + 1, n + 1);

    /* result in seq_t, avoiding overflow by using seq_t arithmetic */
    return factor * bin;
}
