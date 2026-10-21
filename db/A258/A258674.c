/*
 * A258674  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum equal to the antidiagonal sum.
 * https://oeis.org/A258674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A258674: Number of (n+2) × (1+2) 0..1 arrays with no 3 × 3 subblock
   diagonal sum equal to the antidiagonal sum.
   Empirical recurrence:
     a(n) = 2*a(n-1) + 32*a(n-3) + 256*a(n-4)  for n ≥ 5
   Base values:
     a(1)=320, a(2)=1600, a(3)=8320, a(4)=43264
   The implementation uses only integer arithmetic on seq_t.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the first term for n=1. */
    if (n == 1) return 320;
    /* Return second term for n=2. */
    if (n == 2) return 1600;
    /* Return third term for n=3. */
    if (n == 3) return 8320;
    /* Return fourth term for n=4. */
    if (n == 4) return 43264;

    /* Sliding window for a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a1 = 320;          /* a(n-4) when n = 5 -> a1 = a(1)      */
    seq_t a2 = 1600;         /* a(n-3) when n = 5 -> a2 = a(2)      */
    seq_t a3 = 8320;         /* a(n-2) when n = 5 -> a3 = a(3)      */
    seq_t a4 = 43264;        /* a(n-1) when n = 5 -> a4 = a(4)      */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)2 * a4 + (seq_t)32 * a2 + (seq_t)256 * a1;
        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return cur;
}
