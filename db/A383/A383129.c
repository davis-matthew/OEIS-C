/*
 * A383129  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (421*s + 54773) mod 259200 when
 *          started at 1.
 * https://oeis.org/A383129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (421*a(n-1) + 54773) mod 259200, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power_sum.h"

/* Main sequence function.  It works for any n >= 1, in any call order,
   and is efficient even for large n because it uses a logarithmic
   exponentiation routine. */
seq_t a(idx_t n) {
    const seq_t MOD   = (seq_t)259200;
    const seq_t A     = (seq_t)421;
    const seq_t C     = (seq_t)54773;
    const seq_t FIRST = (seq_t)1;

    if (n == 1) return FIRST;          /* offset 1 */

    idx_t k = n - 1;                   /* number of steps from the first element */

    seq_t powA, sumSeries;
    power_sum(A, k, &powA, &sumSeries);

    /* a(n) = powA * a(1) + C * sumSeries  (mod MOD) */
    seq_t result = (powA * FIRST + C * sumSeries) % MOD;
    return result;
}
