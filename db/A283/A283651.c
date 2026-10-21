/*
 * A283651  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283651
 * Formula: a(0) = 1, a(1) = 0, a(2) = 7,
 * a(n) = 2^(n-1) - 1        if n >= 3 and n even
 * a(n) = 3*2^(n-1) - 1      if n >= 3 and n odd
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;

    /* For n >= 3 compute 2^(n-1). */
    seq_t p = pow2(n - 1);          /* 2^(n-1) */

    if ((n & 1) == 0) {            /* n even */
        return p - 1;              /* 2^(n-1) - 1 */
    } else {                       /* n odd */
        return (p << 1) + p - 1;   /* 3*2^(n-1) - 1 */
    }
}
