/*
 * A391887  Alternately add and multiply the last two terms starting with 2,
 *          1.
 * https://oeis.org/A391887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) if n even, a(n) = a(n-1) * a(n-2) if n
 *            odd; a(0)=2, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cache so that repeated calls (even
 * in arbitrary order) are answered in O(1).  The cache is grown
 * geometrically as needed. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;
static idx_t  cache_len = 0;
static idx_t  max_computed = 0;  /* highest index whose value is in the cache */

/* Ensure the cache is large enough to hold index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    idx_t new_len = cache_len ? cache_len << 1 : 2;
    while (new_len <= n) new_len <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache     = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n <= max_computed) return cache[n];

    ensure_cache(n);

    /* Initialise the first two values if not already done */
    if (max_computed < 1) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute values from the first unknown index up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if ((i & 1) == 0)          /* even index: addition */
            cache[i] = cache[i - 1] + cache[i - 2];
        else                       /* odd index: multiplication */
            cache[i] = cache[i - 1] * cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
