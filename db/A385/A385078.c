/*
 * A385078  Consecutive states of the linear congruential pseudo-random number
 *          generator (967*s + 3041) mod 14406 when started at s=1.
 * https://oeis.org/A385078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (967 * a(n-1) + 3041) mod 14406, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 9669, u32 n <= 9669, u64 n <= 9669,
 *            u128 n <= 9669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Start with a(1) = 1 */
    seq_t val = 1;

    /* For n>1 iterate the linear congruential recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = (val * (seq_t)967 + (seq_t)3041) % (seq_t)14406;
    }

    return val;
}
