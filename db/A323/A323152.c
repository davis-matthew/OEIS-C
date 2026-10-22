/*
 * A323152  a(n) = 1 if sigma(n) is divisible by all proper divisors of n, 0
 *          otherwise.
 * https://oeis.org/A323152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if σ(n) is divisible by all proper divisors of n, 0
 *            otherwise
 * Range:     u8 n <= 8107, u16 n <= 8107, u32 n <= 8107, u64 n <= 8107,
 *            u128 n <= 8107, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n == 1 has no proper divisors – the condition is vacuously true. */
    if (n == 1)
        return 1;

    /* Factor n.  factor_u64 fills the arrays with primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Compute σ(n) multiplicatively.  σ(p^e) = 1 + p + … + p^e. */
    uint64_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t term = 1;          /* 1 + p + … + p^e */
        uint64_t p_pow = 1;
        for (uint64_t k = 0; k < e; ++k) {
            p_pow *= p;
            term += p_pow;
        }
        sigma *= term;
    }

    /* Generate all divisors of n.  The divisor count for n ≤ 10 000 is < 512. */
    uint64_t divisors[512];
    size_t sz = 1;
    divisors[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t base_sz = sz;
        uint64_t p_pow = 1;
        for (uint64_t k = 0; k < e; ++k) {
            p_pow *= p;                 /* p^(k+1) */
            for (size_t j = 0; j < base_sz; ++j) {
                divisors[sz++] = divisors[j] * p_pow;
            }
        }
    }

    /* Verify divisibility by every proper divisor. */
    uint64_t nn = n;
    for (size_t i = 0; i < sz; ++i) {
        uint64_t d = divisors[i];
        if (d == nn)          /* skip n itself */
            continue;
        if (sigma % d != 0ULL)
            return 0;
    }

    return 1;
}
