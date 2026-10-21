/*
 * A244478  a(0)=2, a(1)=0, a(2)=2; thereafter a(n) =
 *          a(n-1-a(n-1))+a(n-2-a(n-2)) unless a(n-1) <= n-1 or a(n-2) <= n-2
 *          in which case the sequence terminates.
 * https://oeis.org/A244478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2, a(1)=0, a(2)=2; For n≥3,
 *            a(n)=a(n‑1‑a(n‑1))+a(n‑2‑a(n‑2)) indices are taken non‑negative
 *            as the recurrence guarantees
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* memoization array */
static idx_t  cache_size = 0;       /* number of elements that are already computed */
static size_t cache_cap  = 0;       /* current allocated capacity */

static void grow_cache(idx_t n)
{
    /* make sure the cache can hold indices up to n (inclusive) */
    if (n >= cache_size) {
        if (cache == NULL) {
            /* initial allocation covers indices 0,1,2 */
            cache_cap = (size_t) ((n >= 2) ? n + 1 : 3);
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();               /* memory error */
            /* base values */
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)2;
            cache_size = 3;
        }
        /* enlarge capacity if necessary */
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (n + 1 > new_cap) new_cap <<= 1;   /* double until big enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* memory error */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute all entries from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0 || i == 1 || i == 2) continue;   /* already set */
            idx_t idx1 = (idx_t)(i - 1) - (idx_t)cache[i - 1];
            idx_t idx2 = (idx_t)(i - 2) - (idx_t)cache[i - 2];
            cache[i] = cache[idx1] + cache[idx2];
        }
        cache_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    grow_cache(n);         /* ensure cache holds value for index n */
    return cache[n];
}
