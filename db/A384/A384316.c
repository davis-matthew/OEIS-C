/*
 * A384316  Consecutive states of the linear congruential pseudo-random number
 *          generator 3125*s mod 2^26 when started at s=1.
 * https://oeis.org/A384316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3125 * a(n-1) mod 2^26, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t exp = n - 1;
    const seq_t MOD = (seq_t)((1ULL << 26));
    seq_t result = 1;
    seq_t base   = (seq_t)3125;

    while (exp)
    {
        if (exp & 1)
        {
            uint64_t tmp = (uint64_t)result * (uint64_t)base;
            result = (seq_t)(tmp % (uint64_t)MOD);
        }
        exp >>= 1;
        if (exp)
        {
            uint64_t tmp = (uint64_t)base * (uint64_t)base;
            base = (seq_t)(tmp % (uint64_t)MOD);
        }
    }

    return result;
}
