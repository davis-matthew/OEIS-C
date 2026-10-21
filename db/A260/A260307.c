/*
 * A260307  a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-6) - a(n-7) - a(n-8) +
 *          a(n-9) with a(0) - a(8) as shown below.
 * https://oeis.org/A260307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-6) - a(n-7) - a(n-8) + a(n-9)
 * Initial values: 
 *   a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=5, a(6)=8, a(7)=7, a(8)=10
 */
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static const seq_t init_vals[9] = {1, 2, 3, 4, 6, 5, 8, 7, 10};

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – unlikely in tests */
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
    }

    size_t idx = (size_t)n;
    if (idx < cache_len) return cache[idx];

    /* Grow cache to accommodate index n */
    size_t new_len = idx + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                         /* allocation failure – unlikely */
    cache = tmp;

    /* Compute values on demand using the recurrence */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* compute a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-6)-a(i-7)-a(i-8)+a(i-9) */
        seq_t pos = cache[i-1] + cache[i-2] + cache[i-6] + cache[i-9];
        seq_t neg = cache[i-3] + cache[i-7] + cache[i-8];
        cache[i] = pos - neg;                   /* pos >= neg for all indices */
    }
    cache_len = new_len;
    return cache[idx];
}
