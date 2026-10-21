/*
 * A278963  a(n) is the number of k, 1<=k<=n, such that gcd(n,k) divides
 *          binomial(n,k).
 * https://oeis.org/A278963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for OEIS A278963:  a(n) = #{ k | 1 ≤ k ≤ n  and  gcd(n,k) | C(n,k) }.
 *  This implementation factors gcd(n,k) using a smallest‑prime‑factor table
 *  and checks the p‑adic valuation of C(n,k) by Legendre’s formula.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* declares void build_spf(idx_t limit, idx_t **out); */

static idx_t *spf = NULL;   /* smallest prime factor for every number up to current limit */
static idx_t spf_limit = 0; /* current size of spf table */

/* Euclidean gcd for idx_t (uint64_t). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Computes exponent of prime p in n! (Legendre). */
static inline int exp_in_factorial(idx_t n, idx_t p)
{
    int cnt = 0;
    while (n) {
        n /= p;
        cnt += n;
    }
    return cnt;
}

/* Ensures spf table is built up to 'limit' (inclusive). */
static inline void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Counts the number of k (1 ≤ k ≤ n) such that gcd(n,k) divides binomial(n,k). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n == 0, but harmless */
    ensure_spf(n);                    /* spf table up to n is required for factorization */

    seq_t count = 0;
    /* Temporary arrays for prime factors; n ≤ 10000 ⇒ ≤ 5 distinct primes. */
    idx_t  primes[8];
    int    exps[8];
    size_t factor_cnt;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t g = gcd_idx(n, k);
        if (g == 1) {                 /* gcd = 1 always divides C(n,k) */
            ++count;
            continue;
        }

        /* Factorize g using spf table */
        idx_t temp = g;
        factor_cnt = 0;
        while (temp > 1) {
            idx_t p = spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[factor_cnt]   = p;
            exps[factor_cnt]     = e;
            ++factor_cnt;
        }

        bool divides = true;
        for (size_t i = 0; i < factor_cnt && divides; ++i) {
            idx_t  p   = primes[i];
            int    need = exps[i];
            int    val  = exp_in_factorial(n, p) - exp_in_factorial(k, p) - exp_in_factorial(n - k, p);
            if (val < need)
                divides = false;
        }
        if (divides)
            ++count;
    }
    return count;
}
