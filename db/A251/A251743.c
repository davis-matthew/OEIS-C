/*
 * A251743  Pairs of nodes in a complete binary tree that are at an absolute
 *          height difference of less than 2 from each other.
 * https://oeis.org/A251743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*2^n + 2^(1+2*n) - 5) / 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251743 – pairs of nodes in a complete binary tree at
 * an absolute height difference less than 2. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 3 * 2^n */
    seq_t t1 = 3 * pow2(n);

    /* Compute 2^(1 + 2*n) */
    seq_t t2 = pow2(1 + 2 * n);

    /* Compute (t1 + t2 - 5) / 3 */
    seq_t result = (t1 + t2 - 5) / 3;

    return result;
}
