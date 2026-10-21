/*
 * A273325  Number of endofunctions on [2n] such that the minimal cardinality
 *          of the nonempty preimages equals n.
 * https://oeis.org/A273325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (2*n-1) * C(2*n, n) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute binomial coefficient C(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    /* compute n * (2*n - 1) */
    seq_t second = (seq_t)n * (seq_t)(2 * n - 1);

    return bin * second;
}
