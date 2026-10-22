/*
 * A352863  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n+3,k+3) * a(k).
 * https://oeis.org/A352863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352863
   a(0) = 1
   a(n) = Σ_{k=0}^{n-1} binomial(n+3, k+3) * a(k)  for n ≥ 1
   (uses memoization; O(n²) time, O(n) memory) */

#include <stdlib.h>
#include "../utils/binomial.h"          /* seq_t binomial(idx_t a, idx_t b) */

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* ---------- Memoisation cache ---------- */
static seq_t *cache      = NULL;   /* array of previously computed values   */
static idx_t  cache_len  = 0;      /* number of values that have been set   */
static size_t cache_cap  = 0;      /* current allocated capacity            */

/* free the cache in the unlikely event of a program exit */
static void free_cache(void) __attribute__((destructor));
static void free_cache(void)
{
    if (cache) {
        free(cache);
        cache = NULL;
    }
    cache_len = 0;
    cache_cap = 0;
}

/* ----------------- Main routine ----------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = (size_t)n + 1;
        if (cache_cap == 0) cache_cap = 1;          /* avoid malloc(0) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                       /* allocation failed */
        cache[0] = (seq_t)1;                       /* a(0) = 1 */
        cache_len = 1;
    }

    /* Expand cache if necessary to include n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        /* binomial(i+3, 3) is the first needed coefficient */
        seq_t bin = binomial(i + 3, (idx_t)3);

        for (idx_t k = 0; k < i; ++k) {
            /* Add current term */
            sum += bin * cache[k];

            /* Update binomial for next k (unless at the last) */
            if (k + 1 < i) {
                /* bin(i+3, k+4) = bin(i+3, k+3) * (i - k) / (k + 4) */
                bin = bin * (seq_t)(i - k);
                bin = bin / (seq_t)(k + 4);
            }
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
