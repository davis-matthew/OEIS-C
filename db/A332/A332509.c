/*
 * A332509  a(n) = Sum_{k=1..n} mu(floor(n/k)), where mu = A008683.
 * https://oeis.org/A332509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(floor(n/k)) (mu = Möbius function)
 * Range:     u8 n <= 1118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Compute Möbius values up to "limit" (inclusive).  The array mu must have
 * at least limit+1 elements.  mu[1] = 1, mu[0] = 0, and for n>=2 we use
 * the classic linear sieve algorithm. */
static void compute_mu(int8_t *mu, idx_t limit)
{
    memset(mu, 0, (limit + 1) * sizeof(int8_t));
    if (limit >= 1) mu[1] = 1;

    /* prime sieve structures */
    idx_t   *primes   = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    size_t   primes_cnt = 0;
    uint8_t *is_comp  = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    memset(is_comp, 0, (limit + 1) * sizeof(uint8_t));

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > limit) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    free(primes);
    free(is_comp);
}

/* Static cache for Möbius values.  mu_cache[0..mu_cache_size] is filled. */
static int8_t *mu_cache = NULL;
static idx_t mu_cache_size = 0;

/* Ensure mu_cache contains Möbius values up to n.  Re‑allocates and recomputes
 * if necessary.  The cache is never truncated. */
static void ensure_mu(idx_t n)
{
    if (n <= mu_cache_size) return;

    int8_t *new_cache = (int8_t *)malloc((n + 1) * sizeof(int8_t));
    if (!new_cache) return; /* should never happen in normal usage */

    compute_mu(new_cache, n);

    free(mu_cache);
    mu_cache = new_cache;
    mu_cache_size = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; this handles n==0 safely */

    ensure_mu(n);

    long long sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t v = n / i;           /* floor(n/i) */
        sum += (long long)mu_cache[v];
    }

    return (seq_t)sum;
}
