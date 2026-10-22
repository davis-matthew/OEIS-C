/*
 * A387748  a(n) = (1/7) * Sum_{k=0..n-1} binomial(7*n,7*k+1).
 * https://oeis.org/A387748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/7) * Σ_{k=0..n-1} binomial(7*n, 7*k+1) Recurrence:
 *            a(n) = 71*a(n-1) + 7585*a(n-2) - 36991*a(n-3) - 128*a(n-4)
 *            Initial terms: a(0)=0, a(1)=1, a(2)=431, a(3)=36825
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache[0] = (seq_t)0;
        if (n >= 1) cache[1] = (seq_t)1;
        if (n >= 2) cache[2] = (seq_t)431;
        if (n >= 3) cache[3] = (seq_t)36825;
        /* compute remaining terms up to n */
        for (idx_t i = 4; i <= n; ++i) {
            seq_t term = (seq_t)71 * cache[i - 1];
            term += (seq_t)7585 * cache[i - 2];
            term -= (seq_t)36991 * cache[i - 3];
            term -= (seq_t)128 * cache[i - 4];
            cache[i] = term;
        }
        cached_len = n + 1;
        return cache[n];
    }

    /* if requested index already cached, return it */
    if (cached_len > n) {
        return cache[n];
    }

    /* extend cache to cover index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache = NULL;
        cached_len = 0;
        return (seq_t)0;
    }
    cache = tmp;

    /* fill missing indices */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)431;
        } else if (i == 3) {
            cache[i] = (seq_t)36825;
        } else {
            seq_t term = (seq_t)71 * cache[i - 1];
            term += (seq_t)7585 * cache[i - 2];
            term -= (seq_t)36991 * cache[i - 3];
            term -= (seq_t)128 * cache[i - 4];
            cache[i] = term;
        }
    }
    cached_len = n + 1;
    return cache[n];
}
