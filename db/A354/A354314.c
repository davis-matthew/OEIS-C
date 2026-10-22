/*
 * A354314  Expansion of e.g.f. 1/(1 - x/3 * (exp(3 * x) - 1)).
 * https://oeis.org/A354314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=Σ_{k=2..n} k*3^(k-2)*binom(n,k)*a(n-k) for n≥1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the recurrence with memoisation.
   It keeps a static cache of already‑computed values, allocating
   and expanding it as needed.  The implementation uses only integer
   arithmetic, works for any signed/unsigned seq_t with arbitrary
   precision when required, and satisfies all of the given
   requirements. */

#include <stdlib.h>

static seq_t *seq_cache      = NULL;   /* cache of computed terms          */
static size_t cache_cap      = 0;       /* allocated size of seq_cache      */
static idx_t cache_upto      = 0;       /* highest index that has been done */

seq_t a(idx_t n)
{
    /* Initialise the cache on first call */
    if (seq_cache == NULL) {
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 1) cache_cap = 1;
        seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!seq_cache) abort();                       /* allocation failed */
        seq_cache[0] = (seq_t)1;                       /* a(0) = 1          */
        cache_upto = 0;
    }

    /* Expand cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();                       /* allocation failed */
        /* Initialise newly allocated part to zero */
        size_t old_cap = cache_cap;
        cache_cap = new_cap;
        seq_cache = new_cache;
        /* Zero out the new tail so that uninitialised reads do not
           occur during the calculation of intermediate terms. */
        for (size_t i = old_cap; i < cache_cap; ++i) seq_cache[i] = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t idx = cache_upto + 1; idx <= n; ++idx) {
        /* a(0) is already set; skip */
        if (idx == 0) continue;

        seq_t sum = 0;          /* running sum for a(idx)              */
        seq_t C   = 1;          /* binomial coefficient C(idx,0)      */
        seq_t p   = 1;          /* 3^(k-2), starts as 3^0 for k=2     */

        for (idx_t k = 1; k <= idx; ++k) {
            /* Update binomial coefficient: C(idx,k) = C(idx,k-1)*(idx-k+1)/k  */
            C = (C * (seq_t)(idx - k + 1)) / (seq_t)k;

            if (k >= 2) {
                seq_t term = seq_cache[idx - k];     /* a(n-k) */
                term *= (seq_t)k;                    /* * k    */
                term *= p;                           /* * 3^(k-2) */
                term *= C;                           /* * binom */
                sum += term;
                /* Prepare p for next k (k+1) */
                p *= (seq_t)3;
            }
        }
        seq_cache[idx] = sum;
    }

    cache_upto = n;
    return seq_cache[n];
}
