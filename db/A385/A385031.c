/*
 * A385031  Consecutive states of the linear congruential pseudo-random number
 *          generator 171*s mod 30269 when started at s=1.
 * https://oeis.org/A385031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 171 * a(n-1) mod 30269 for n > 1. The sequence
 *            is periodic with period 30268
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 safely */

    /* Reduce the exponent modulo the period (30268). */
    const idx_t period = 30268;
    idx_t exp = (n - 1) % period;   /* exponent for 171^exp (mod 30269) */

    uint64_t result = 1;
    uint64_t base   = 171;
    const uint64_t mod = 30269;

    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return (seq_t)result;
}
