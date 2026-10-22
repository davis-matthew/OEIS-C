/*
 * A336804  a(n) = (n!)^2 * Sum_{k=0..n} 2^(n-k) / (k!)^2.
 * https://oeis.org/A336804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n^2 * a(n-1) + 1, with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence supplied in the OEIS entry for
   sequence A336804.  The calculation is done iteratively, using only
   integer arithmetic, and works for any build where seq_t supports
   the usual +, -, *, and == operators. */
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)2 * (seq_t)i * (seq_t)i; /* 2 * i^2 */
        result = mult * result + (seq_t)1;
    }
    return result;
}
