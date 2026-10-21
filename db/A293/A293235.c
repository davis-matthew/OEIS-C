/*
 * A293235  a(n) is the sum of proper divisors of n that are square.
 * https://oeis.org/A293235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A010052(d)*d
 * Range:     u8 n <= 449, u16 n <= 9941, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the sum of all proper square divisors of n */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) = 0, a(0) is irrelevant */

    seq_t sum = 0;                 /* result accumulator (seq_t) */

    /* iterate over possible square roots s, with s^2 <= n */
    for (uint64_t s = 1; s <= n / s; ++s) {
        uint64_t sq = s * s;       /* square divisor candidate */

        /* exclude n itself (proper divisors only) */
        if (sq == n) continue;

        /* if sq divides n, add it to the sum */
        if (n % sq == 0) {
            sum += (seq_t)sq;
        }
    }
    return sum;
}
