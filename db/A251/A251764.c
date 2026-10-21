/*
 * A251764  10-step Fibonacci sequence starting with 0,0,0,1,0,0,0,0,0,0.
 * https://oeis.org/A251764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+10) = a(n)+a(n+1)+…+a(n+9), a(0)=0, a(1)=0, a(2)=0, a(3)=1,
 *            a(4)=0,…,a(9)=0
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   alloc_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cap.h"

static seq_t *cache = NULL;       /* stored values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of values actually stored */

/* Grow the cache to at least new_cap elements, keeping existing data. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t old_cap = cache_cap;
    cache_cap = new_cap;
    if (cache == NULL) {
        cache = alloc_cap(cache_cap);
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return; /* allocation failure – silently ignore */
        cache = tmp;
    }
    /* new elements are zero‑initialised by calloc/realloc behaviour on
       extended memory in typical implementations; if not, they already
       hold zero from allocation. */
}

/* Initialise the cache with the first ten terms. */
static void init_cache(void)
{
    grow_cache(10);
    /* a(3) = 1, all others 0 */
    cache[3] = (seq_t)1;
    cache_len = 10;
}

seq_t a(idx_t n)
{
    /* initialise cache on first invocation */
    if (cache == NULL) init_cache();

    /* if the requested index is already known, return it */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* extend cache to accommodate n */
    if ((size_t)n + 1 > cache_cap) grow_cache((size_t)n + 1);

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        /* sum the previous ten terms */
        for (size_t j = 0; j < 10; ++j) {
            sum += cache[i - 10 + j];
        }
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
