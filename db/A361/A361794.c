/*
 * A361794  Sum of the cubes d^3 of the divisors d satisfying d^2|n.
 * https://oeis.org/A361794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d^2|n} d^3
 * Range:     u8 n <= 48, u16 n <= 1599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t pow3(seq_t base) {
    return base * base * base;
}

seq_t a(idx_t n) {
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];
        idx_t k = (e / 2) + 1;           // number of terms in the geometric sum

        seq_t cur = 1;
        seq_t sum = 0;
        seq_t p3 = pow3(p);

        for (idx_t j = 0; j < k; ++j) {
            sum += cur;
            cur *= p3;
        }
        result *= sum;
    }
    return result;
}
