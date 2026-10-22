/*
 * A389452  Sum of the divisors of the largest p-full divisor of n (a term of
 *          A054744 that divides n).
 * https://oeis.org/A389452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (multiplicative): a(p^e) = (p^(e+1)-1)/(p-1) if e>=p, otherwise 1 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e >= p) {
            seq_t pow = seq_pow((seq_t)p, (idx_t)(e + 1));   /* p^(e+1) */
            seq_t sigma = (pow - 1) / (seq_t)(p - 1);        /* 1 + p + ... + p^e */
            result *= sigma;
        }
    }
    return result;
}
