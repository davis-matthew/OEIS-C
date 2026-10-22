/*
 * A383865  The number of divisors d of n having the property that for every
 *          prime p dividing n the p-adic valuation of d is either 0 or an
 *          infinitary divisor of the p-adic valuation of n.
 * https://oeis.org/A383865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (1 + 2^{ω(e)}), where ω(e) is the number of
 *            distinct prime factors of e. a(1) = 1. The function is
 *            multiplicative and computed via prime factorization and simple
 *            prime counting
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t temp = e;
        uint64_t distinct = 0;

        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                ++distinct;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) ++distinct;

        seq_t mult = 1 + ((seq_t)1 << distinct);
        result *= mult;
    }
    return result;
}
