/*
 * A348341  a(n) is the number of noninfinitary divisors of n.
 * https://oeis.org/A348341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) - 2^{∑ popcount(e_i)} where n = ∏ p_i^{e_i} and d(n)
 *            is the number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute the number of noninfinitary divisors of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */

    uint64_t m = (uint64_t)n;      /* copy of n for factoring */
    seq_t tau = 1;                 /* divisor count */
    unsigned int sum_pop = 0;      /* sum of popcounts of exponents */

    /* trial division up to sqrt(m) */
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            unsigned int e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            tau *= (seq_t)(e + 1);
            /* popcount of e: e fits in unsigned int */
            sum_pop += __builtin_popcount(e);
        }
    }
    /* Remaining prime factor (exponent 1) */
    if (m > 1) {
        tau *= 2;
        sum_pop += 1;
    }

    /* compute 2^{sum_pop} as repeated multiplication to avoid
       shifting beyond the width of seq_t. */
    seq_t pow2 = 1;
    for (unsigned int k = 0; k < sum_pop; ++k) {
        pow2 *= 2;
    }

    return tau - pow2;
}
