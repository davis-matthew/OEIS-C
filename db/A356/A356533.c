/*
 * A356533  a(n) = sigma_2(n)^2.
 * https://oeis.org/A356533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_2(n)^2 = A001157(n)^2
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    /* factorize n into prime powers */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute multiplicative product of (sum of squares of divisors of each
       prime power)² */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* sigma_2(p^e) = (p^(2e+2) - 1) / (p^2 - 1) */
        seq_t exp = (seq_t)(2 * e + 2);
        seq_t num = big_pow(p, exp) - 1;
        seq_t den = (p * p) - 1;
        seq_t sum = num / den;          /* integer division */

        result *= sum * sum;            /* square for this prime power */
    }
    return result;
}
