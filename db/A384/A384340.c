/*
 * A384340  Consecutive states of the linear congruential pseudo-random number
 *          generator (314159221*s+211324863) mod 10^9 when started at s=1.
 * https://oeis.org/A384340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (314159221*a(n-1)+211324863) mod 1000000000 for
 *            n > 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Global cache for memoisation */
static seq_t *cache = NULL;      /* 1‑based indexing (cache[0] unused) */
static idx_t  cache_size = 0;    /* highest computed index */
static idx_t  cache_cap  = 0;    /* allocated capacity (1‑based) */

/* forward declaration of helper */
static void ensure_cache(idx_t req);

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, harmless */
    ensure_cache(n);
    return cache[n];
}

/* helper to grow the cache up to req */
static void ensure_cache(idx_t req)
{
    if (req <= cache_size) return;

    /* new capacity: power of two at least req */
    idx_t new_cap = (cache_cap > 0 ? cache_cap : 1);
    while (new_cap < req) new_cap <<= 1;

    /* reallocate (size new_cap+1 for 1‑based index) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    } else {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    }
    if (cache == NULL) exit(1);   /* allocation failed */

    cache_cap = new_cap;

    /* compute missing terms */
    for (idx_t i = cache_size + 1; i <= req; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            cache[i] = (cache[i - 1] * (seq_t)314159221ULL
                       + (seq_t)211324863ULL) % (seq_t)1000000000ULL;
        }
    }
    cache_size = req;
}
