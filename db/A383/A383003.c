/*
 * A383003  a(n) = Sum_{d|n} (-n)^(d-1).
 * https://oeis.org/A383003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} (-n)^(d-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for arbitrary n (n >= 1) in O(#divisors of n) time. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper: accumulate the sum of base^(divisor-1) for all divisors
 * generated from the prime factorisation. */
static void sum_div_terms(idx_t idx,
                          uint64_t curr,
                          const uint64_t *primes,
                          const uint64_t *exps,
                          size_t pf_cnt,
                          seq_t base,
                          seq_t *result)
{
    if (idx == pf_cnt) {
        /* curr is a divisor of the original n */
        seq_t term = big_pow(base, (seq_t)(curr - 1));
        *result += term;
        return;
    }

    uint64_t prime = primes[idx];
    uint64_t exp   = exps[idx];
    uint64_t mult  = 1;

    for (uint64_t e = 0; e <= exp; ++e) {
        sum_div_terms(idx + 1,
                      curr * mult,
                      primes,
                      exps,
                      pf_cnt,
                      base,
                      result);
        mult *= prime;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, but safeguard */

    /* Factorise n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(n, primes, exps, &pf_cnt);

    /* Prepare base (-n) as seq_t */
    seq_t base = -(seq_t)n;

    seq_t result = 0;
    sum_div_terms(0, 1, primes, exps, pf_cnt, base, &result);
    return result;
}
