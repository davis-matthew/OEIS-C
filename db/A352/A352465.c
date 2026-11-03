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
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Worth noting: this uses the provided `binomial` and `ensure_cache`
   helpers from the utilities. */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
