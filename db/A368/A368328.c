/*
 * A368328  The number of terms of A054743 that divide n.
 * https://oeis.org/A368328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} ( 1 if e ≤ p else e - p + 1 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */
uint64_t _smallest_prime_factor(uint64_t);

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0, return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Factorization arrays – 64 primes are enough for 64‑bit integers. */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t      cnt = 0;

    /* Decompose n into prime powers. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e > p) {
            /* (e - p + 1) always fits in uint64_t; cast to seq_t for safety. */
            result *= (seq_t)(e - p + 1);
        }
        /* If e ≤ p, the factor is 1; product remains unchanged. */
    }
    return result;
}
