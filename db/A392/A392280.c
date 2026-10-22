/*
 * A392280  The maximum exponent in the prime factorization of the smallest
 *          multiple of n whose prime factorization exponents are all powers
 *          of 2.
 * https://oeis.org/A392280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{p^e || n} { smallest power of two ≥ e } ; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;           /* a(1) = 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t max_exp = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t p = (seq_t)1;
        while (p < e) p <<= 1;
        if (p > max_exp) max_exp = p;
    }
    return max_exp;
}
