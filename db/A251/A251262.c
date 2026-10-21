/*
 * A251262  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 5*a(n-3) + 2*a(n-4) (offset 1: a(1)
 *            = 26, a(2) = 114, a(3) = 480, a(4) = 2058)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the n-th term of OEIS A251262.
   n must be >= 1. For n == 0 the function returns 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n <= 0 */

    /* Base values */
    const seq_t a1 = 26;
    const seq_t a2 = 114;
    const seq_t a3 = 480;
    const seq_t a4 = 2058;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    seq_t t4 = a1;   /* a(n-4) */
    seq_t t3 = a2;   /* a(n-3) */
    seq_t t2 = a3;   /* a(n-2) */
    seq_t t1 = a4;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)(5 * t1 - 2 * t2 - 5 * t3 + 2 * t4);
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = cur;
    }

    return t1;
}
