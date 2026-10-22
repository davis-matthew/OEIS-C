/*
 * A318583  a(1) = a(2) = 1; for n > 2, a(n+2) = Sum_{d|n} mu(n/d)*a(d).
 * https://oeis.org/A318583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   for n>2, a(n+2) = Σ_{d|n} μ(n/d)·a(d) a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=0 (offset 1)
 * Range:     i8 n <= 73, i16 n <= 473, i32 n <= 2813, i64 n <= 9105,
 *            i128 n <= 9105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size (≥ max index + 1) */
static idx_t cache_computed = 0;  /* largest index whose value is stored */

/* static storage for Möbius μ values */
static int8_t *mu = NULL;
static size_t mu_cap = 0;         /* allocated size (≥ max index + 1) */

/* sieve Möbius up to n, allocating memory as needed */
static void ensure_mu_upto(idx_t n)
{
    if (mu_cap >= (size_t)n + 1) return;          /* already enough space */

    size_t new_cap = (size_t)n + 1;
    int8_t *new_mu =
        (int8_t *)realloc(mu, new_cap * sizeof(int8_t));  /* C++ cast */
    if (!new_mu) exit(1);
    mu = new_mu;
    mu_cap = new_cap;

    /* recompute μ with a linear sieve (O(n)) */
    int *spf = (int *)calloc(new_cap, sizeof(int));
    if (!spf) exit(1);
    int *primes = (int *)malloc(new_cap * sizeof(int));
    if (!primes) exit(1);
    size_t pcnt = 0;

    mu[0] = 0;
    mu[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {
            spf[i] = (int)i;
            mu[i] = -1;
            primes[pcnt++] = (int)i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            int p   = primes[j];
            int64_t v = (int64_t)p * i;
            if (v > n) break;
            int vi = (int)v;
            spf[vi] = p;
            if (i % p == 0) {
                mu[vi] = 0;
                break;
            } else {
                mu[vi] = (int8_t)(-mu[i]);
            }
        }
    }
    free(spf);
    free(primes);
}

/* ensure cache capacity up to n (inclusive) and initialize base values once */
static void ensure_cache_upto(idx_t n)
{
    if (cache_cap >= (size_t)n + 1) return;   /* already enough space */

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < (size_t)n + 1) new_cap = n + 1;

    seq_t *new_cache =
        (seq_t *)realloc(cache, new_cap * sizeof(seq_t));  /* C++ cast */
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;

    if (cache_computed == 0) {   /* first allocation: set base values */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        if (cache_cap > 4) cache[4] = 0;
        cache_computed = 4;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset; return 0 */

    ensure_cache_upto(n);
    /* μ is needed up to (n-2) because recurrence uses n = i-2 */
    idx_t mu_needed = (n > 2) ? n - 2 : 0;
    ensure_mu_upto(mu_needed);

    if (cache_computed >= n) return cache[n];   /* already computed */

    /* compute values up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i <= 4) {          /* base values already set */
            if (i <= cache_cap) cache[i] = (i == 1 || i == 2 || i == 3) ? 1
                                 : ((i == 4) ? 0 : cache[i]);  /* safety */
            continue;
        }

        idx_t k = i - 2;               /* recurrence index */
        seq_t sum = 0;

        /* iterate over divisors of k */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;

                /* term for divisor d1 */
                seq_t term1 = (seq_t)mu[(size_t)(k / d1)] * cache[d1];
                sum += term1;

                if (d1 != d2) {
                    /* term for divisor d2 */
                    seq_t term2 = (seq_t)mu[(size_t)(k / d2)] * cache[d2];
                    sum += term2;
                }
            }
        }
        cache[i] = sum;
    }

    cache_computed = n;
    return cache[n];
}
