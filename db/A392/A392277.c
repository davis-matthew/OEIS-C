/*
 * A392277  The maximum exponent in the prime factorization of the cube root
 *          of the largest cube dividing n.
 * https://oeis.org/A392277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(max exponent in prime factorization of n / 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stddef.h>

seq_t a(idx_t n) {
    /* For n < 2 the maximum exponent is 0, hence a(n) = 0. */
    if (n < 2) return (seq_t)0;

    /* Use factor_u64 to factor n and find the largest exponent. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t max_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > max_exp) max_exp = exps[i];
    }

    return (seq_t)(max_exp / 3);
}
