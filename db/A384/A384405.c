/*
 * A384405  Consecutive internal states of the linear congruential
 *          pseudo-random number generator 69621 * s mod (2^31-1) when started
 *          at s=1.
 * https://oeis.org/A384405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 69621 * a(n-1) mod (2^31-1), a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t M = 2147483647ULL;   /* 2^31-1 */
    const uint64_t base0 = 69621ULL;
    uint64_t exp = (uint64_t)n - 1;
    uint64_t base = base0;
    uint64_t res = 1;
    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % M;
        base = (base * base) % M;
        exp >>= 1;
    }
    return (seq_t)res;
}
