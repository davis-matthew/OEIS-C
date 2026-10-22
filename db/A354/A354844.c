/*
 * A354844  a(n) = n! * Sum_{d|n} (n/d)^d / (d! * (n/d)!).
 * https://oeis.org/A354844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} (n/d)^d /(d! * (n/d)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary utilities */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* compute exponent of prime p in m! via Legendre's formula */
static int64_t exp_in_fact(idx_t m, uint64_t p)
{
    int64_t sum = 0;
    uint64_t power = p;
    while (power <= m) {
        sum += m / power;
        if (power > m / p) break;
        power *= p;
    }
    return sum;
}

/* compute term for a single divisor d of n:
   term = n! * (n/d)^d / (d! * (n/d)!)     */
static seq_t term_for_divisor(idx_t n, idx_t d,
                              uint64_t *primes, size_t prime_cnt,
                              const int64_t *exp_n)
{
    idx_t k = n / d;                    /* quotient n/d */
    size_t i, j;

    /* allocate temporary exponent arrays */
    int64_t *exp_d      = (int64_t *)malloc(sizeof(int64_t) * prime_cnt);
    int64_t *exp_kfact  = (int64_t *)malloc(sizeof(int64_t) * prime_cnt);
    int64_t *exp_kprime = (int64_t *)calloc(sizeof(int64_t), prime_cnt);

    /* exponents of primes in d! and k! */
    for (i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        exp_d[i]     = exp_in_fact(d, p);
        exp_kfact[i] = exp_in_fact(k, p);
    }

    /* factor k to obtain exponents of primes in k itself */
    uint64_t pf[64], ex[64];
    size_t pf_cnt;
    factor_u64((uint64_t)k, pf, ex, &pf_cnt);
    for (i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        uint64_t e = ex[i];
        for (j = 0; j < prime_cnt; ++j) {
            if (primes[j] == p) {
                exp_kprime[j] = (int64_t)e;
                break;
            }
        }
    }

    /* construct the term from prime exponents */
    seq_t term = (seq_t)1;
    for (i = 0; i < prime_cnt; ++i) {
        int64_t total = exp_n[i] - exp_d[i] - exp_kfact[i] + (int64_t)d * exp_kprime[i];
        if (total > 0) {
            seq_t pw = seq_pow((seq_t)primes[i], (idx_t)total);
            term      = seq_mul(term, pw);
        }
    }

    free(exp_d);
    free(exp_kfact);
    free(exp_kprime);
    return term;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* outside offset – undefined */

    /* get primes up to n */
    uint64_t *primes;
    size_t   prime_cnt;
    build_primes_upto(n, &primes, &prime_cnt);

    /* pre‑compute exponent of each prime in n! */
    int64_t *exp_n = (int64_t *)malloc(sizeof(int64_t) * prime_cnt);
    for (size_t i = 0; i < prime_cnt; ++i) {
        exp_n[i] = exp_in_fact(n, primes[i]);
    }

    seq_t sum = (seq_t)0;

    /* iterate through all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t t = term_for_divisor(n, d, primes, prime_cnt, exp_n);
            sum += t;                /* the addition operator is available for seq_t */

            if (d != n / d) {        /* handle the paired divisor */
                idx_t d2 = n / d;
                t = term_for_divisor(n, d2, primes, prime_cnt, exp_n);
                sum += t;
            }
        }
    }

    free(primes);
    free(exp_n);
    return sum;
}
