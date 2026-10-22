/*
 * A367819  Expansion of e.g.f. exp(1 - 4*x - exp(x)).
 * https://oeis.org/A367819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -4*a(n-1) - sum_{k=0}^{n-1} binom(n-1,k) *
 *            a(n-1-k) (n >= 1)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses O(n^2) arithmetic, memoised in a static cache.  Works for any
   seq_t type and any idx_t type (uint64_t).  All allocations use malloc/realloc
   with explicit cast to seq_t*.  */

#include <stdlib.h>

/* Forward declaration – free_cache is not required but good practice */
static void free_cache(void);

/* Static cache for computed terms */
static seq_t *cache = NULL;   /* array of a(i) values */
static idx_t cache_size = 0;  /* length allocated in cache */

/* Expand cache up to index n, computing new terms on demand */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;          /* already have required terms */

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) { /* allocation failed – leave cache unchanged */
        return;
    }
    cache = tmp;

    /* compute new terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0) {                    /* a(0) = 1 */
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom = 1;                 /* binom(i-1,0) = 1 */

        for (idx_t k = 0; k < i; ++k)
        {
            /* term = binom(i-1,k) * a(i-1-k) */
            sum += binom * cache[i - 1 - k];

            /* update binom for next k:
               binom(i-1,k+1) = binom(i-1,k) * (i-1-k) / (k+1)   exact division */
            if (k + 1 < i) {
                binom = binom * (seq_t)(i - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }

        cache[i] = -4 * cache[i - 1] - sum;
    }

    cache_size = new_size;
}

/* Public interface required by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
