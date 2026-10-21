/*
 * A289107  a(n) = 2*a(n-1) - a(n-3) for n >= 5, a(0) = 2, a(1) = 4, a(2) = 7,
 *          a(3) = 12, a(4) = 22.
 * https://oeis.org/A289107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n >= 5, a(0)=2, a(1)=4, a(2)=7, *
 *            a(3)=12, a(4)=22
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[5] = { (seq_t)2, (seq_t)4, (seq_t)7,
                               (seq_t)12, (seq_t)22 };

/* Return the nth term of OEIS sequence A289107. */
seq_t a(idx_t n)
{
    /* Handle the five explicitly given initial values. */
    if (n < 5)
        return init[(size_t)n];

    seq_t a2 = init[2]; /* a(n-3) for current n */
    seq_t a3 = init[3]; /* a(n-2) for current n (unused in recurrence) */
    seq_t a4 = init[4]; /* a(n-1) for current n */
    seq_t res = a4;     /* placeholder for a(n) */

    /* Compute successive terms up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        res = 2 * a4 - a2;   /* a(i) = 2*a(i-1) - a(i-3) */
        /* Shift the window for the next iteration.  a2 becomes a3,
           a3 becomes a4, and a4 becomes the newly computed res. */
        a2 = a3;
        a3 = a4;
        a4 = res;
    }
    return res;
}
