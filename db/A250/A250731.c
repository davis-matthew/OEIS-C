/*
 * A250731  Number of (2+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) (n > 4) with initial
 *            values: a(1) = 18, a(2) = 46, a(3) = 110, a(4) = 257
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Return the known initial values directly */
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)46;
    if (n == 3) return (seq_t)110;
    if (n == 4) return (seq_t)257;

    /* Keep the last four terms of the recurrence */
    seq_t a1 = (seq_t)18;   /* a(n-3) */
    seq_t a2 = (seq_t)46;   /* a(n-2) */
    seq_t a3 = (seq_t)110;  /* a(n-1) */
    seq_t a4 = (seq_t)257;  /* a(n)   */

    /* Iterate n = 5 … n */
    for (idx_t k = 5; k <= n; ++k) {
        /*   t = 2*a4 + 2*a3 - 3*a2   */
        seq_t t = a4 * 2 + a3 * 2;   /* 2*a(n) + 2*a(n-1) */
        t -= a2 * 3;                /* subtract 3*a(n-2)   */

        /* shift window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = t;
    }

    return a4;   /* a(n) */
}
