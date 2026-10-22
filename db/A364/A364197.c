/*
 * A364197  a(n+1) = a(|n-a(n)^2|) + 1, a(0) = 0.
 * https://oeis.org/A364197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0
   a(n+1) = a(|n - a(n)^2|) + 1
   (OEIS A364197) */

#include <stdlib.h>

/* static cache of already computed values
 * cache_len == number of filled entries, 0 means no allocation yet
 */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute A364197(n) for all n >= 0, memoizing results.
 * Works for any seq_t/idx_t combination that satisfies the
 * type requirements above. */
seq_t a(idx_t n)
{
    /* initialise for the very first call */
    if (n == 0) {
        if (cache_len == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            cache[0] = (seq_t)0;
            cache_len = 1;
        }
        return cache[0];
    }

    /* enlarge cache if necessary (do not change cache_len until values are computed) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
    } else if (cache_len <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* cache_len stays unchanged – we have not filled new slots yet */
    }

    /* compute missing values from cache_len up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t prev = i - 1;                      /* index of a(i-1)          */
        seq_t ai_minus1 = cache[prev];           /* a(i-1)                   */
        /* compute a(i-1)^2 safely – cast to idx_t to avoid overflow in seq_t */
        idx_t m2 = (idx_t)((idx_t)ai_minus1 * (idx_t)ai_minus1);

        /* |prev - m2| as idx_t */
        idx_t diff = (prev >= m2) ? (prev - m2) : (m2 - prev);

        /* build a(i) */
        cache[i] = cache[diff] + (seq_t)1;
    }

    /* all values up to n are now computed */
    cache_len = n + 1;
    return cache[n];
}
