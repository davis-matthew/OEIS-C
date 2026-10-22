/*
 * A359175  a(n) = binomial(2*n-2,n) - 2*(n-1).
 * https://oeis.org/A359175
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(2*n-2, n) - 2*(n-1)
 * (Sequence A359175, offset 3)
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3; for smaller n
     * we return 0 as a harmless default, although such calls do
     * not occur in tests. */
    if (n < 3)
        return 0;

    /* Compute the binomial coefficient C(2n-2, n). */
    seq_t comb = binomial(2 * n - 2, n);

    /* Subtract 2*(n-1).  All operations are performed in seq_t
     * to avoid overflow in intermediate results. */
    seq_t sub = (seq_t)2 * ((seq_t)n - 1);

    return comb - sub;
}
