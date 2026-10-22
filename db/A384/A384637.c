/*
 * A384637  Consecutive states of the linear congruential pseudo-random number
 *          generator 5^13*s mod 2^35 when started at s=1.
 * https://oeis.org/A384637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=5^13*a(n-1) mod 2^35 (n>1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants: multiplier m = 5^13 = 1220703125,
       modulus M = 2^35 = 34359738368 */
    const uint64_t M = 1ULL << 35;
    const uint64_t m = 1220703125ULL;

    if (n == 0)
        return 0;  /* below offset, value irrelevant */

    uint64_t result = 1;
    uint64_t base   = m;
    idx_t    exp    = n - 1;

    while (exp) {
        if (exp & 1ULL) {
            unsigned __int128 t = (unsigned __int128)result * base;
            result = (uint64_t)(t % M);
        }
        exp >>= 1;
        if (exp) {
            unsigned __int128 t = (unsigned __int128)base * base;
            base = (uint64_t)(t % M);
        }
    }
    return (seq_t)result;
}
