/*
 * A341583  Geometric length of the solution to the Towers of Hanoi exchanging
 *          disks puzzle with 3 pegs and n disks.
 * https://oeis.org/A341583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + 2*a(n-4) - 4*a(n-5) *
 *            Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=8, a(4)=18
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* required for size_t */

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t init[5] = {
        (seq_t)0,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)3,  /* a(2) */
        (seq_t)8,  /* a(3) */
        (seq_t)18  /* a(4) */
    };

    if (n < 5) {
        return init[(size_t)n];
    }

    /* 5-term sliding window to compute the recurrence */
    seq_t buf[5];
    for (size_t i = 0; i < 5; ++i) {
        buf[i] = init[i];
    }

    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        next = ((seq_t)3) * buf[4]          /* 3*a(i-1) */
             - buf[3]                      /* - a(i-2) */
             - ((seq_t)2) * buf[2]         /* - 2*a(i-3) */
             + ((seq_t)2) * buf[1]         /* + 2*a(i-4) */
             - ((seq_t)4) * buf[0];        /* - 4*a(i-5) */

        /* shift window left, insert new term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
