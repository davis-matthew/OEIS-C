/*
 * A287431  Number of connected dominating sets in the n-gear graph.
 * https://oeis.org/A287431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3) + 4*a(n-4) with a(1)=7,
 *            a(2)=21, a(3)=45, a(4)=129 (valid for n≥1, recurrence for n>4)
 *            Computed by O(n) iterative recurrence with memoisation
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[i] holds a(i) */
static size_t cache_cap = 0;     /* allocated size of cache array    */
static size_t cache_max = 0;     /* largest n that has been computed */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? 2 * cache_cap : 32;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – keep existing cache */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of range – undefined, return 0 */

    ensure_cache(n);

    /* initialise first four terms on first use */
    if (cache_max == 0) {
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)45;
        cache[4] = (seq_t)129;
        cache_max = 4;
    }

    /* compute values from cache_max+1 up to n */
    for (idx_t i = (idx_t)(cache_max + 1); i <= n; ++i) {
        seq_t v = (seq_t)4 * cache[i - 1] + (seq_t)4 * cache[i - 4];
        v -= cache[i - 2];
        v -= (seq_t)6 * cache[i - 3];
        cache[i] = v;
    }
    if (n > (idx_t)cache_max)
        cache_max = (size_t)n;

    return cache[n];
}
