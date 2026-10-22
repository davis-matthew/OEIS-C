/*
 * A331661  E.g.f. A(x) satisfies: d/dx A(x) = 1 + (1/(1 + x)) * A(x/(1 + x)).
 * https://oeis.org/A331661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n+1) = Σ_{k=0}^{n-1} (-1)^k · binomial(n,k)^2 · k! ·
 *            a(n-k)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment.  The function
   binomial(a,b) is also supplied. */
extern seq_t binomial(idx_t a, idx_t b);

/* --------- Memoization -------------------------------------------- */

/* Cached sequence values, zero‑based array (index 0 unused). */
static seq_t *cache  = NULL;   /* array holding a(0)…a(n)           */
static size_t cache_cap = 0;    /* allocated size in elements         */
static idx_t cached_up = 0;     /* largest n that has been computed   */

/* Ensure the cache can hold indices up to `n`. */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);  /* +1 because we keep index 0      */
    if (needed <= cache_cap)
        return;

    /* Reallocate using the big‑integer helper if available; fall back
       to malloc/realloc when that helper isn’t provided. */
#if defined(SEQ_REALLOC_AVAILABLE)
    seq_t *new_cache = seq_realloc(cache, cache_cap, needed);
#else
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         needed * sizeof(seq_t));
#endif

    if (!new_cache) abort();       /* unrecoverable failure           */

    /* Initialize newly added slots to zero. */
    for (size_t i = cache_cap; i < needed; ++i)
        new_cache[i] = (seq_t)0;

    cache       = new_cache;
    cache_cap   = needed;
}

/* Compute all terms up to and including `n`. */
static void compute_up_to(idx_t n)
{
    if (n <= cached_up)
        return;                        /* nothing new to add            */

    ensure_cache_size(n);

    /* Set a(1) on first use. */
    if (cached_up == 0) {
        cache[1] = (seq_t)1;
        cached_up = 1;
    }

    /* Compute terms one by one. */
    for (idx_t m = cached_up + 1; m <= n; ++m) {
        idx_t N   = m - 1;              /* current n in the recurrence     */
        seq_t sum = (seq_t)0;

        seq_t bin  = (seq_t)1;          /* binomial(N,0) */
        seq_t fact = (seq_t)1;          /* 0!             */

        /* k runs from 0 to N-1. */
        for (idx_t k = 0; k < N; ++k) {
            /* term = bin^2 * fact * a(N-k) with sign (-1)^k */
            seq_t term = bin * bin;              /* binomial(N,k)^2 */
            term      = term * fact;             /* * k!            */
            term      = term * cache[N - k];     /* * a(N-k)        */

            if (k & 1)   /* odd k => negative sign */
                sum = sum - term;
            else
                sum = sum + term;

            /* Advance binomial and factorial to next k. */
            if (k + 1 < N) {
                seq_t num = (seq_t)(N - k);   /* N - k          */
                seq_t den = (seq_t)(k + 1);   /* k + 1          */
                bin = (bin * num) / den;     /* binomial(N,k+1)*/
                fact = fact * (k + 1);       /* (k+1)!         */
            }
        }

        cache[m] = sum;
    }

    cached_up = n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1 – a(0) is unused */
        return (seq_t)0;

    compute_up_to(n);
    return cache[n];
}
