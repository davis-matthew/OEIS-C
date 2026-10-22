/*
 * A344547  Lexicographically earliest sequence of nonnegative terms forming a
 *          clockwise square spiral when nothing else is read except the
 *          parity of the terms (see the Comments section).
 * https://oeis.org/A344547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Let k be the largest integer such that start_k = 1 when k = 1
 *            start_k = (k/2)^2 + 1 if k is even start_k = ((k-1)/2) *
 *            ((k+1)/2) + 1 if k is odd Then a(1) = 0 if n is a start index
 *            (i.e. n == start_k) and n > 1 a(n) = 2*k - 3 else a(n) = 2*(n -
 *            k) This yields the lexicographically earliest sequen...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   start_of_segment
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/start_of_segment.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Find the largest k with start_k <= n */
    idx_t k = 1;
    while (start_of_segment(k) <= n)
        ++k;
    --k;                                   /* now k is the desired segment index */

    /* Determine if n is exactly a start index > 1 */
    if (n == start_of_segment(k))
    {
        /* odd parity slot */
        return (seq_t)((seq_t)2 * (seq_t)k - (seq_t)3);
    }
    else
    {
        /* even parity slot */
        return (seq_t)((seq_t)2 * ((seq_t)n - (seq_t)k));
    }
}
