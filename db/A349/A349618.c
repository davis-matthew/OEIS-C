/*
 * A349618  Dirichlet convolution of arithmetic derivative with A325126
 *          [Dirichlet inverse of rad(n)].
 * https://oeis.org/A349618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(d) * A325126(n/d) (Dirichlet
 *            convolution)
 * Range:     u8 n <= 255, u16 n <= 8768, u32 n <= 8768, u64 n <= 8768,
 *            u128 n <= 8768, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate divisors and accumulate the sum. */
static void dfs(size_t pos, uint64_t *primes, uint64_t *exps, size_t cnt,
                uint64_t cur_d, uint64_t *exp_cur, int64_t *sum_result)
{
    if (pos == cnt) {
        /* Compute contribution for this divisor cur_d with exponents exp_cur. */
        int64_t der = 0;        /* arithmetic derivative of cur_d */
        int64_t b_val = 1;      /* Dirichlet inverse of rad for n/cur_d */

        /* arithmetic derivative: Σ e_i * cur_d / p_i */
        for (size_t i = 0; i < cnt; i++) {
            uint64_t p = primes[i];
            uint64_t e = exp_cur[i];
            if (e > 0) {
                der += (int64_t)e * (int64_t)(cur_d / p);
            }
        }

        /* Dirichlet inverse of rad: multiplicative, b(p^k) computed on the fly. */
        for (size_t i = 0; i < cnt; i++) {
            uint64_t p = primes[i];
            uint64_t e_orig = exps[i];
            uint64_t e_used = exp_cur[i];
            uint64_t e_rem  = e_orig - e_used;
            if (e_rem == 0) continue;

            /* Compute b(p^e_rem) by recursion formula:
             * b(p^1)   = -p
             * b(p^k) = -p * Σ_{j=0}^{k-1} b(p^j)   (k ≥ 2)
             */
            int64_t sum = 1;       /* Σ_{j=0}^{k-1} b(p^j), starting with b(p^0)=1 */
            int64_t b_pk = 0;      /* b(p^k) */
            for (uint64_t j = 1; j <= e_rem; j++) {
                b_pk = -(int64_t)p * sum;
                sum += b_pk;
            }
            b_val *= b_pk;
        }

        *sum_result += der * b_val;
        return;
    }

    /* Recurse over exponent of primes[pos] */
    uint64_t p = primes[pos];
    uint64_t maxe = exps[pos];
    uint64_t d = cur_d;
    for (uint64_t e = 0; e <= maxe; e++) {
        exp_cur[pos] = e;
        dfs(pos + 1, primes, exps, cnt, d, exp_cur, sum_result);
        if (e < maxe) d *= p;   /* multiply by p for next exponent */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Allocate array to hold current exponent for each prime */
    uint64_t *exp_cur = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!exp_cur) {
        /* In case of allocation failure, return zero (unlikely). */
        return (seq_t)0;
    }

    int64_t total = 0;
    dfs(0, primes, exps, prime_cnt, (uint64_t)1, exp_cur, &total);

    free(exp_cur);
    return (seq_t)total;
}
