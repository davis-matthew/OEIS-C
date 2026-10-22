/*
 * A365725  G.f. satisfies A(x) = 1 + x^3*A(x)^4*(1 + x*A(x)).
 * https://oeis.org/A365725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k,n-3*k) *
 *            binomial(n+k+1,k) / (n+k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* Cached values of the sequence */
static size_t cache_cap = 0;     /* Allocated size of the cache array */
static idx_t cached_max = (idx_t)(-1);  /* Largest n for which cache[i] is filled */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL || cache_cap <= (size_t)n + 1)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute a single value a(n) using the provided binomial summation. */
static seq_t compute_a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        idx_t r = n - 3 * k;
        if (r > k) continue;            /* binomial(k, r) would be zero */
        seq_t t = binomial(k, r);
        t *= binomial(n + k + 1, k);
        t /= (seq_t)(n + k + 1);
        result += t;
    }
    return result;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Compute and store any missing values up to n. */
    for (idx_t i = (cached_max == (idx_t)(-1)) ? 0 : cached_max + 1;
         i <= n; ++i)
    {
        cache[i] = compute_a(i);
    }
    cached_max = n;

    return cache[n];
}
