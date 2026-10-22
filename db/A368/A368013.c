/*
 * A368013  Expansion of e.g.f. 1/(1 - exp(x) + exp(2*x)).
 * https://oeis.org/A368013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} (1 - 2^k) * C(n,k) * a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;      /* table of computed a(n) values          */
static size_t cache_cap = 0;     /* capacity of cache (in elements)       */
static idx_t  next_to_compute = 0;/* first index that is not yet computed  */

/* Grow the cache so that it can hold at least (n+1) elements. */
static void grow_cache(idx_t n)
{
    /* if capacity already >= n+1, nothing to do */
    if (cache_cap > (size_t)n) return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* power‑of‑two growth */

    seq_t *new_ptr = cache
        ? (seq_t *)realloc(cache, new_cap * sizeof(*cache))
        : (seq_t *)malloc(new_cap * sizeof(*cache));
    if (!new_ptr) exit(EXIT_FAILURE);             /* allocation failed   */

    cache     = new_ptr;
    cache_cap = new_cap;
}

/* Ensure that all values a(0)..a(n) have been computed. */
static void ensure_computed(idx_t n)
{
    if (n < next_to_compute) return;          /* already done           */

    grow_cache(n);                            /* guarantees storage   */

    if (next_to_compute == 0) {               /* initialise a(0)      */
        cache[0] = (seq_t)1;
        next_to_compute = 1;
    }

    for (idx_t i = next_to_compute; i <= n; ++i) {
        seq_t sum  = (seq_t)0;
        seq_t comb = (seq_t)1;   /* C(i,0)          */
        seq_t pow2 = (seq_t)2;   /* 2^1 for k = 1   */

        for (idx_t k = 1; k <= i; ++k) {
            /* binomial coefficient update: C(i,k) = C(i,k-1) * (i-k+1) / k */
            comb = comb * (seq_t)(i - k + 1) / (seq_t)k;

            /* factor (1 - 2^k) */
            seq_t term_factor = (seq_t)1 - pow2;

            /* term = term_factor * comb * a(i-k) */
            seq_t term = term_factor * comb * cache[i - k];
            sum = sum + term;

            /* advance pow2 for next k (2^k) */
            pow2 = pow2 * (seq_t)2;
        }
        cache[i] = sum;
    }
    next_to_compute = n + 1;   /* everything up to n is now ready   */
}

seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
