/*
 * A384432  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for Borland C and C++ when started
 *          at 1.
 * https://oeis.org/A384432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (22695477 * a(n-1) + 1) mod 2^31 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t v = 1;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        v = (22695477ULL * v + 1) & 0x7FFFFFFFULL; /* modulo 2^31 */
    }
    return (seq_t)v;
}
