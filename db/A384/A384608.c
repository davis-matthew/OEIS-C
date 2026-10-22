/*
 * A384608  Consecutive states of the linear congruential pseudo-random number
 *          generator (129*s+27098671125) mod 2^35 when started at s=1.
 * https://oeis.org/A384608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (129*a(n-1) + 27098671125) mod 2^35, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    const uint64_t MOD_MASK = ((uint64_t)1 << 35) - 1ULL;
    uint64_t state = 1;          /* a(1) */
    if (n == 1) return (seq_t)state;

    for (idx_t i = 2; i <= n; ++i) {
        state = (state * 129ull + 27098671125ull) & MOD_MASK;
    }
    return (seq_t)state;
}
