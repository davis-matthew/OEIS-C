/*
 * A250952  Number of (n+1) X (4+1) 0..1 arrays with no 2 X 2 subblock having
 *          its maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 57*a(n-2) - 46*a(n-3) + 30*a(n-4) (n >= 5)
 *            Initial values: a(1)=780, a(2)=19208, a(3)=472712, a(4)=11633448
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but handle 0 safely */

    /* Return the first four known terms directly */
    switch (n) {
    case 1: return (seq_t)780;
    case 2: return (seq_t)19208;
    case 3: return (seq_t)472712;
    case 4: return (seq_t)11633448;
    default:
        break;
    }

    /* Initialize the four most recent terms:
       v1 = a(n-4), v2 = a(n-3), v3 = a(n-2), v4 = a(n-1) for the
       current iteration.  For n=5, these are a1,a2,a3,a4. */
    seq_t v1 = (seq_t)780;          /* a(1) */
    seq_t v2 = (seq_t)19208;        /* a(2) */
    seq_t v3 = (seq_t)472712;       /* a(3) */
    seq_t v4 = (seq_t)11633448;     /* a(4) */

    /* Iterate from 5 up to n, using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        /* Compute new_term = 27*v4 - 57*v3 - 46*v2 + 30*v1 */
        seq_t sum = v4 * (seq_t)27;      /* 27*a(n-1) */
        sum += v1 * (seq_t)30;          /* +30*a(n-4) */

        seq_t sub = v3 * (seq_t)57;      /* 57*a(n-2) */
        sub += v2 * (seq_t)46;          /* +46*a(n-3) */

        seq_t new_term = sum - sub;     /* ensure sum >= sub for unsigned types */

        /* Shift the window for the next iteration */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = new_term;
    }

    return v4; /* v4 holds a(n) after loop */
}
