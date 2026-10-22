/*
 * A373364  a(n) = gcd(A001414(n), A003415(n)), where A001414 is the sum of
 *          prime factors with repetition, and A003415 is the arithmetic
 *          derivative.
 * https://oeis.org/A373364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001414(n), A003415(n)) Computes the sum of prime
 *            factors with multiplicity (A001414) and the arithmetic
 *            derivative (A003415) of n, then returns their greatest common
 *            divisor
 * Range:     u8 n <= 512, u16 n <= 8252, u32 n <= 8252, u64 n <= 8252,
 *            u128 n <= 8252, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;

    uint64_t value = (uint64_t)n;

    /* arrays large enough for any n < 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(value, primes, exps, &cnt);

    seq_t sum_pf = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum_pf += (seq_t)exps[i] * (seq_t)primes[i];
    }

    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = (seq_t)(value / primes[i]) * (seq_t)exps[i];
        deriv += part;
    }

    return gcd(sum_pf, deriv);
}
