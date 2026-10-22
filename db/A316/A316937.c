/*
 * A316937  a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) for n > 2, a(0)=3, a(1)=10,
 *          a(2)=26.
 * https://oeis.org/A316937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3); a(0)=3, a(1)=10, a(2)=26
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoised cache of already computed terms.  */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* Grow the cache until it can hold at least n+1 terms.  */
static void ensure_cache_size(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    if (cache == NULL) {
        cache_cap = needed > 16 ? needed : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (needed > cache_cap) {
        while (cache_cap < needed) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    for (size_t i = cache_size; i < needed; ++i) {
        seq_t val;
        if (i == 0)           val = (seq_t)3;
        else if (i == 1)      val = (seq_t)10;
        else if (i == 2)      val = (seq_t)26;
        else                  val = 3 * cache[i-1] - cache[i-2] - 2 * cache[i-3];
        cache[i] = val;
    }
    cache_size = needed;
}

seq_t a(idx_t n) {
    ensure_cache_size(n);
    return cache[n];
}
