/*
 * A357719  Expansion of e.g.f. cos( 2 * log(1+x) ).
 * https://oeis.org/A357719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(2*n - 3)*a(n-1) - (n^2 - 4*n + 8)*a(n-2), with a(0) =
 *            1, a(1) = 0
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoized sequence values */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of computed entries */

/* Ensure the cache has space for at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 8;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* allocation failed */
    }
    cache = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* Expand cache up to index n if needed */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        ensure_cache_capacity(needed);
        for (size_t i = cache_len; i < needed; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)0;
            } else {
                seq_t prev1 = cache[i - 1];
                seq_t prev2 = cache[i - 2];
                idx_t ii = (idx_t)i;
                seq_t ti = (seq_t)ii;
                seq_t coef1 = (seq_t)(2 * ti - 3);          /* 2*i - 3 */
                seq_t coef2 = (seq_t)(ti * ti - 4 * ti + 8); /* i^2 - 4*i + 8 */
                seq_t part1 = -(coef1 * prev1);
                seq_t part2 = -(coef2 * prev2);
                cache[i] = part1 + part2;
            }
        }
        cache_len = needed;
    }
    return cache[(size_t)n];
}
