/*
 * A251269  Number of (2+1) X (n+1) 0..1 arrays with no 2 X 2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 9*a(n-4) + 2*a(n-5),
 *            with a(1)=35, a(2)=114, a(3)=313, a(4)=772, a(5)=1775
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* stored values a(1)...a(cache_size) */
static size_t cache_size = 0;        /* number of values currently stored */
static size_t cache_cap   = 0;        /* allocated capacity for cache */

/* initial values */
static const seq_t base[5] = {
    35,   /* a(1) */
    114,  /* a(2) */
    313,  /* a(3) */
    772,  /* a(4) */
    1775  /* a(5) */
};

static void ensure_cache(idx_t n)
{
    /* Make sure cache holds values up to index n (1‑based) */
    if (cache_size >= (size_t)n) return;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;  /* in a full program you handle out‑of‑memory */
    } else if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first five terms if not already done */
    if (cache_size == 0) {
        for (size_t i = 0; i < 5 && i < cache_cap; ++i) {
            cache[i] = base[i];
        }
        cache_size = 5;
    }

    /* Generate missing terms using the recurrence */
    for (idx_t k = (idx_t)(cache_size + 1); k <= n; ++k) {
        /* k is 1‑based index; cache indices are 0‑based */
        seq_t v =  6 * cache[k-2]
                 -14 * cache[k-3]
                 +16 * cache[k-4]
                 - 9 * cache[k-5]
                 + 2 * cache[k-6];
        cache[k-1] = v;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, just return 0 */
    if (n <= 5) return base[n-1];

    ensure_cache(n);
    return cache[n-1];
}
