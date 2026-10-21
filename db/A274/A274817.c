/*
 * A274817  a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) for n>3, a(0)=1, a(1)=-1,
 *          a(2)=4, a(3)=8.
 * https://oeis.org/A274817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) for n>3 with a(0)=1,
 *            a(1)=-1, a(2)=4, a(3)=8
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 31, i64 n <= 62, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

seq_t a(idx_t n)
{
    /* handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* iterate using only the last four terms */
    seq_t v0 = (seq_t)1;   /* a(n-4) */
    seq_t v1 = (seq_t)-1;  /* a(n-3) */
    seq_t v2 = (seq_t)4;   /* a(n-2) */
    seq_t v3 = (seq_t)8;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = 2*a(i-1) - a(i-3) + 2*a(i-4) */
        cur = ((seq_t)2 * v3) - v1 + ((seq_t)2 * v0);

        /* Shift the sliding window */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = cur;
    }
    return cur;
}
