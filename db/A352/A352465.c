/*
 * A352465  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(2*n,2*k)^2 * k *
 *          a(n-k).
 * https://oeis.org/A352465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/n) * Σ_{k=1..n} binomial(2*n, 2*k)^2 * k *
 *            a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Worth noting: this uses the provided `binomial` and `ensure_cache`
   helpers from the utilities. */

#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

static seq_t *a_cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;       /* current capacity of a_cache      */
static idx_t max_cached = 0;       /* highest index already computed   */

seq_t a(idx_t n)
{
    /* Make sure the cache can hold indices up to n */
    ensure_cache(n, &a_cache, &cache_cap);

    /* Initialise a(0) once */
    if (max_cached == 0)
    {
        a_cache[0] = (seq_t)1;
        max_cached = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
        {
            seq_t bin  = binomial(2 * i, 2 * k);
            seq_t term = bin * bin;         /* binomial squared   */
            term = term * (seq_t)k;         /* times k            */
            term = term * a_cache[i - k];   /* times a(n-k)       */
            sum = sum + term;               /* accumulate         */
        }
        a_cache[i] = sum / i;               /* divide by n        */
    }
    max_cached = n;
    return a_cache[n];
}
