/*
 * A251963  Numbers n such that the sum of the triangular numbers T(n) and
 *          T(n+1) is equal to an octagonal number N(m) for some m.
 * https://oeis.org/A251963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) Initial values: a(1)=0,
 *            a(2)=14, a(3)=208
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* array of computed terms */
static size_t cache_cap = 0;           /* allocated capacity */
static idx_t cache_len = 0;            /* number of terms currently stored (<= cache_cap) */

/* Ensure terms up to index n are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;               /* no terms to compute */

    /* Allocate or grow the cache array if needed */
    if (cache_len == 0) {
        cache_cap = n > 3 ? n : 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    } else if (cache_cap < (size_t)n) {
        cache_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Compute new terms beyond what is already stored */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)14;
        } else if (i == 2) {
            cache[i] = (seq_t)208;
        } else {
            cache[i] = 15 * cache[i - 1] - 15 * cache[i - 2] + cache[i - 3];
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1; undefined for 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
