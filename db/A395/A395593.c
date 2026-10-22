/*
 * A395593  a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=1, a(1)=8, a(2)=64, a(3)=72.
 * https://oeis.org/A395593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) with initial values a(0)=1,
 *            a(1)=8, a(2)=64, a(3)=72
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)72;

    seq_t a0 = (seq_t)1;   /* a(i-4) */
    seq_t a1 = (seq_t)8;   /* a(i-3) */
    seq_t a2 = (seq_t)64;  /* a(i-2) */
    seq_t a3 = (seq_t)72;  /* a(i-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 + a1 + a0; /* a(i) = a(i-1) + a(i-3) + a(i-4) */
        a0 = a1;           /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
