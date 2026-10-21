/*
 * A278417  a(n) = n*((2+sqrt(3))^n + (2-sqrt(3))^n)/2.
 * https://oeis.org/A278417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((2+sqrt(3))^n + (2-sqrt(3))^n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute T(n) = (2+sqrt(3))^n + (2-sqrt(3))^n.
       T(0) = 2, T(1) = 4, and for k ≥ 2: T(k) = 4*T(k-1) - T(k-2). */
    seq_t t;
    if (n == 0) {
        t = (seq_t)2;
    } else if (n == 1) {
        t = (seq_t)4;
    } else {
        seq_t prev2 = (seq_t)2;
        seq_t prev1 = (seq_t)4;
        seq_t cur;
        for (idx_t i = 2; i <= n; ++i) {
            cur = (seq_t)4 * prev1 - prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        t = prev1;
    }

    /* a(n) = n * T(n) / 2.  T(n) is always even, so we divide first. */
    seq_t t_half = t / 2;
    return (seq_t)n * t_half;
}
