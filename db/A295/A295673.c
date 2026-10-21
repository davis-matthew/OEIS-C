/*
 * A295673  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 4, a(1) = 3, a(2) =
 *          2, a(3) = 1.
 * https://oeis.org/A295673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) a(0) = 4, a(1) = 3, a(2) = 2,
 *            a(3) = 1 (recurrence valid for all n ≥ 4)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return 4;
    if (n == 1) return 3;
    if (n == 2) return 2;
    if (n == 3) return 1;

    /* Cached array of previously computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0; /* first unused index (length) */
    static idx_t cache_cap   = 0; /* allocated capacity */

    /* Initial allocation if this is the first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* safety, unlikely to happen */
        cache[0] = 4; cache[1] = 3;
        cache[2] = 2; cache[3] = 1;
        cache_size = 4;
    }

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* safety */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }
    cache_size = n + 1;   /* now we have values 0..n */

    return cache[n];
}
