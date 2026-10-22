/*
 * A345275  a(n) = Sum_{d|n} (n-d)^sigma(n/d).
 * https://oeis.org/A345275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n-d)^sigma(n/d) (sigma = sum of divisors)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive enumeration of divisors */
static void dfs(const uint64_t *primes, const uint64_t *exps, size_t prime_cnt,
                size_t idx, uint64_t d, uint64_t *exp_d, seq_t n, seq_t *sum)
{
    if (idx == prime_cnt) {
        /* compute sigma(n/d) = Π_i sigma(p_i^{exps[i]-exp_d[i]}) */
        seq_t sigma_nd = (seq_t)1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t rem = exps[i] - exp_d[i];
            seq_t s = (seq_t)1, term = (seq_t)1;
            uint64_t p   = primes[i];
            for (uint64_t e = 0; e < rem; ++e) {
                term *= (seq_t)p;
                s    += term;
            }
            sigma_nd *= s;
        }
        seq_t base  = (seq_t)n - (seq_t)d;
        seq_t tterm = big_pow(base, sigma_nd);
        *sum += tterm;
        return;
    }

    const uint64_t p       = primes[idx];
    uint64_t      p_power  = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        exp_d[idx] = e;
        dfs(primes, exps, prime_cnt, idx + 1,
            d * p_power, exp_d, n, sum);
        p_power *= p;   /* next exponent of p */
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   prime_cnt;

    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t sum    = 0;
    uint64_t exp_d[20];
    dfs(primes, exps, prime_cnt, 0, (uint64_t)1, exp_d, (seq_t)n, &sum);
    return sum;
}
