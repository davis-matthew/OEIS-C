/*
 * A253673  Indices of centered triangular numbers (A005448) that are also
 *          centered octagonal numbers (A016754).
 * https://oeis.org/A253673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 98*a(n-2) – 98*a(n-3) – a(n-4) + a(n-5) Initial
 *            values: a(1)=1, a(2)=16, a(3)=65, a(4)=1520, a(5)=6321 Sequence
 *            is indexed starting at 1. Implemented with an O(n) recurrence
 *            that works for any order of calls
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – undefined, return 0 */
    if (n < 1) return (seq_t)0;

    /* Fast return for the first five terms */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)16;
        case 3: return (seq_t)65;
        case 4: return (seq_t)1520;
        case 5: return (seq_t)6321;
        default: break;
    }

    /* Use a sliding window of the last five terms */
    seq_t a1 = (seq_t)1;    /* a(n-5) */
    seq_t a2 = (seq_t)16;   /* a(n-4) */
    seq_t a3 = (seq_t)65;   /* a(n-3) */
    seq_t a4 = (seq_t)1520; /* a(n-2) */
    seq_t a5 = (seq_t)6321; /* a(n-1) */
    seq_t an;

    /* Compute terms from 6 up to n */
    for (idx_t i = 6; i <= n; ++i) {
        an = a5 + ( (seq_t)98 * a4 ) - ( (seq_t)98 * a3 ) - a2 + a1;
        /* Advance the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
