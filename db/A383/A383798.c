/*
 * A383798  Consecutive states of the linear congruential pseudo-random number
 *          generator for SIMSCRIPT II when started at 1.
 * https://oeis.org/A383798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 630360016 * a(n-1) mod (2^31-1), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^31-1 is a Mersenne prime */
    const seq_t MOD  = (seq_t)2147483647U;
    const seq_t MULT = (seq_t)630360016U;   /* multiplier for the recurrence */

    if (n <= 1) return (seq_t)1;           /* a(1) = 1 */

    idx_t exp    = n - 1;                  /* compute MULT^(n-1) mod MOD */
    seq_t result = (seq_t)1;
    seq_t base   = MULT;

    while (exp > 0) {
        if (exp & 1U)
            result = (seq_t)(((uint64_t)result * (uint64_t)base) % (uint64_t)MOD);
        base = (seq_t)(((uint64_t)base * (uint64_t)base) % (uint64_t)MOD);
        exp >>= 1;
    }
    return result;
}
