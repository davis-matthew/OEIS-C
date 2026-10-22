/*
 * A383645  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (17405*s+10395331) mod 2^24 when
 *          started at s=1.
 * https://oeis.org/A383645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17405*a(n-1)+10395331) mod 2^24, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t a_n  = (seq_t)1;
    const seq_t A = (seq_t)17405;
    const seq_t B = (seq_t)10395331;
    const seq_t M = ((seq_t)1 << 24) - 1;   /* 2^24-1 mask */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (a_n * A + B) & M;
    }
    return a_n;
}
