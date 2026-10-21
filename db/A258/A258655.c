/*
 * A258655  a(n) = A256357(n^2), where exp( Sum_{n>=1} A256357(n)*x^n/n ) = 1
 *          + Sum_{n>=1} x^(n^2) + x^(2*n^2).
 * https://oeis.org/A258655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -sigma(n^2) + Σ_{d|n^2, d≡2 (mod 4)} d + Σ_{d|n^2,
 *            d≡1,4,7 (mod 8)} 2d
 * Range:     i8 n <= 9, i16 n <= 119, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1.  Uses divisor enumeration over n². */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void generate_divisors(size_t idx, seq_t cur,
                              const uint64_t *pr, const uint64_t *exp,
                              size_t total,
                              seq_t *sigma, seq_t *s4, seq_t *s8p)
{
    if (idx == total) {
        *sigma += cur;
        /* d ≡ 2 (mod 4) */
        if ((cur & 3ULL) == 2ULL)
            *s4 += cur;
        /* d ≡ 1,4,7 (mod 8) */
        seq_t mod8 = cur & 7ULL;
        if (mod8 == 1ULL || mod8 == 4ULL || mod8 == 7ULL)
            *s8p += cur;
        return;
    }
    uint64_t p = pr[idx];
    seq_t pow = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        generate_divisors(idx + 1, cur * pow,
                          pr, exp, total, sigma, s4, s8p);
        pow *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* Factor n to get prime powers of n² */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* exponents for n² are doubled */
    uint64_t exp2[64];
    for (size_t i = 0; i < pf_cnt; ++i)
        exp2[i] = 2 * exps[i];

    seq_t sigma = 0, s4 = 0, s8p = 0;
    generate_divisors(0, (seq_t)1, primes, exp2, pf_cnt,
                      &sigma, &s4, &s8p);

    seq_t result = -sigma + s4 + 2 * s8p;
    return result;
}
