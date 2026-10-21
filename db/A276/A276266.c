/*
 * A276266  a(0) = a(1) = a(2) = 1; for n>2, a(n) = ( a(n-1)*a(n-2) + 1 )^2 /
 *          a(n-3).
 * https://oeis.org/A276266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((a(n-1)*a(n-2)+1)^2) / a(n-3) with a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The definition uses only integer arithmetic, so a direct
   iterative implementation works for any seq_t that can store
   the intermediate values.  No memoisation or lookup tables
   are required; the sequence can be recomputed in O(n) time
   for each call.  This is fast enough for n up to about 10 000
   on typical hardware, even with arbitrary‑precision integers. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0 || n == 1 || n == 2) {
        return (seq_t)1;
    }

    /* Use three sliding variables to keep the last three terms. */
    seq_t a_p3 = (seq_t)1;  /* a(n-3)   */
    seq_t a_p2 = (seq_t)1;  /* a(n-2)   */
    seq_t a_p1 = (seq_t)1;  /* a(n-1)   */
    seq_t a_cur = (seq_t)1; /* placeholder for a(n) */

    /* Compute the sequence iteratively from 3 up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t t = a_p1 * a_p2 + (seq_t)1;   /* a(n-1)*a(n-2)+1   */
        a_cur = (t * t) / a_p3;             /* (t^2)/a(n-3)      */

        /* Shift the window for the next iteration. */
        a_p3 = a_p2;
        a_p2 = a_p1;
        a_p1 = a_cur;
    }

    /* a_cur now holds a(n). */
    return a_cur;
}
