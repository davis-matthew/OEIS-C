/*
 * A264788  a(n) is the number of circles added at n-th iteration of the
 *          pattern starting with 2 circles. (See comment.)
 * https://oeis.org/A264788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 5 Initial values:
 *            a(0)=2, a(1)=2, a(2)=4, a(3)=4, a(4)=6, a(5)=10
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* first six terms are known a priori */
    static const seq_t init[6] = { (seq_t)2, (seq_t)2, (seq_t)4,
                                   (seq_t)4, (seq_t)6, (seq_t)10 };

    if (n < 6) {
        return init[(size_t)n];
    }

    /* buffer that holds the last five values: a(i-5) ... a(i-1) */
    seq_t last[5];
    last[0] = init[1];          /* a(1) */
    last[1] = init[2];          /* a(2) */
    last[2] = init[3];          /* a(3) */
    last[3] = init[4];          /* a(4) */
    last[4] = init[5];          /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t new_val = last[4]            /* a(i-1) */
                       + last[2]            /* a(i-3) */
                       - last[1];           /* a(i-4) */

        /* shift the window: drop a(i-5), move others left, insert new_val */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = new_val;
    }

    /* after the loop, last[4] holds a(n) */
    return last[4];
}
