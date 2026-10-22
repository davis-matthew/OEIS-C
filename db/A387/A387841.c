/*
 * A387841  Number of mutual-visibility sets in the complete tripartite graph
 *          K_{n,n,n}.
 * https://oeis.org/A387841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387841: a(n) = 8^n - 3*2^n + 3*n + 5  for n >= 2,  a(1) = 8 */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 8, behaviour for n < 1 is irrelevant */
        return (seq_t)8;

    seq_t pow8 = pow_uint((uint64_t)8, n);
    seq_t pow2 = pow_uint((uint64_t)2, n);

    seq_t term = pow8 - (seq_t)3 * pow2;
    term += (seq_t)3 * (seq_t)n;
    term += (seq_t)5;

    return term;
}
