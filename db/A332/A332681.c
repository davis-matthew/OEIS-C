/*
 * A332681  a(n) = Sum_{k=1..n} mu(k) * ceiling(n/k)^2.
 * https://oeis.org/A332681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} μ(k) * ceil(n/k)^2, offset 1 (a(1)=1)
 * Range:     u8 n <= 21, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches the Möbius function up to the maximum n
   requested so far, using a simple sieve.  The sum is then evaluated
   in O(n) time.  The code works for all supported seq_t/idx_t
   configurations (including arbitrary-precision seq_t). */

#include <stdint.h>
#include <stdlib.h>

/* Static cache for μ(k) values; μ(0) is unused. */
static signed char *mu_cache = NULL;
static idx_t        mu_cache_size = 0;

/* Ensure mu_cache is large enough to index up to n (inclusive). */
static void ensure_mu_cache(idx_t n)
{
    if (n < mu_cache_size) return;          /* already sufficient */

    /* Allocate a new array of length n+1 (indices 0..n). */
    idx_t new_size = n + 1;
    signed char *new_mu = (signed char *)malloc(new_size * sizeof(signed char));
    if (!new_mu) return;                    /* allocation failure */

    /* Allocate an array for smallest prime factors (spf). */
    idx_t *spf = (idx_t *)calloc(new_size, sizeof(idx_t));
    if (!spf) { free(new_mu); return; }

    /* Compute spf using a simple O(n log log n) sieve. */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i <= n / i) {              /* avoid overflow on i*i */
                idx_t start = i * i;
                for (idx_t j = start; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Compute μ(k) from the spf array. */
    new_mu[0] = 0;                          /* dummy */
    new_mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        idx_t j = i / p;
        if (j % p == 0)
            new_mu[i] = 0;
        else
            new_mu[i] = -new_mu[j];
    }

    free(spf);

    /* Replace the old cache.  The old cache will be freed automatically
       when the program exits or is reallocated. */
    free(mu_cache);
    mu_cache      = new_mu;
    mu_cache_size = new_size;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* indices below offset are undefined */

    ensure_mu_cache(n);
    if (!mu_cache) return 0;                /* allocation failure */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        signed char mk = mu_cache[k];
        if (mk == 0) continue;              /* no contribution */

        idx_t ceil_div = (n + k - 1) / k;   /* ceil(n/k) */
        seq_t sq = (seq_t)ceil_div * (seq_t)ceil_div;

        if (mk > 0)
            sum += sq;
        else
            sum -= sq;
    }
    return sum;
}
