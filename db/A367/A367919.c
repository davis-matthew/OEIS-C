/*
 * A367919  Expansion of e.g.f. exp(4*(exp(x) - 1) - x).
 * https://oeis.org/A367919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + 4 * Σ_{k=1..n} binomial(n-1,k-1) * a(n-k) a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* memoised values of a(n)                 */
static size_t cache_cap = 0;     /* allocated capacity of `cache`           */
static idx_t  last_computed = 0;/* highest n for which cache[n] is valid   */

static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *new_ptr = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_ptr[i] = cache[i];
            delete[] cache;
        }
        cache = new_ptr;
        cache_cap = new_cap;
#else
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();               /* out‑of‑memory */
        cache = new_ptr;
        cache_cap = new_cap;
#endif
    }
}

seq_t a(idx_t n)
{
    /* base case */
    if (cache_cap == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* if higher n requested, extend cache */
    if (cache_cap <= (size_t)n)
        ensure_cache(n);

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;               /* C(i-1,0) */

        for (idx_t j = 0; j < i; ++j)          /* j = k-1 */
        {
            sum += binom * cache[i - j - 1];

            if (j + 1 < i)
                binom = (binom * (seq_t)(i - 1 - j)) / (seq_t)(j + 1);
        }

        cache[i] = -cache[i - 1] + (seq_t)4 * sum;
    }

    last_computed = n;
    return cache[n];
}
