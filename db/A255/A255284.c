/*
 * A255284  a(n) = A255283(2^n-1).
 * https://oeis.org/A255284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A255283(2^n - 1) Recurrence: a(0)=1, a(1)=7, a(2)=31 a(n)
 *            = 4*a(n-1) + 7*a(n-2) - 28*a(n-3) for n > 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small n handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)31;

    /* Recurrence: keep last three terms */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)31;  /* a(n-1) */
    seq_t an = a2;         /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 4 * a2 + 7 * a1 - 28 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
