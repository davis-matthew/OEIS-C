/*
 * A362057  Number of compositions of n that are anti-palindromic modulo 3.
 * https://oeis.org/A362057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - a(n-4), a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=3
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* Use a simple circular buffer of size 4 to hold the previous terms */
    seq_t a0 = (seq_t)1;       /* a(n-4) */
    seq_t a1 = (seq_t)1;       /* a(n-3) */
    seq_t a2 = (seq_t)1;       /* a(n-2) */
    seq_t a3 = (seq_t)3;       /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + (seq_t)3 * a1 - a0;  /* a(n) = a(n-1) + 3*a(n-3) - a(n-4) */
        a0 = a1;  /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
