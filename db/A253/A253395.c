/*
 * A253395  Number of (n+1) X (6+1) 0..1 arrays with every 2 X 2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6) for n
 *            > 16; initial values a(1)…a(16) are given in the OEIS entry
 * Range:     u8 none, u16 n <= 23, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    0,          /* index 0 unused */
    476, 320, 419, 632, 932, 1318, 1855, 2528, 3408, 4498,
    5864, 7521, 9542, 11949, 14824, 18197
};

seq_t a(idx_t n)
{
    /* cache of computed terms, 1‑based indexing (cache[0] unused) */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;   /* number of entries allocated (== cache_len) */

    if (n < 1) return 0;          /* undefined, but avoid UB */

    /* Initialise cache on first call: at least indices 1…16. */
    if (cache_len == 0) {
        idx_t needed = (n > 16) ? n : 16;   /* allocate up to the larger of n or 16 */
        cache_len = needed + 1;             /* +1 for index 0 unused */
        cache = (seq_t*)calloc((size_t)cache_len, sizeof(seq_t));
        cache[0] = 0;                      /* safety */
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init_vals[i];
    }

    /* If already computed, return the value. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate new indices. */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure – unlikely for the given limits */
    cache = tmp;
    cache_len = new_len;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i <= 16) /* already set */ continue;
        cache[i] = 4*cache[i-1] - 5*cache[i-2] + 5*cache[i-4] - 4*cache[i-5] + cache[i-6];
    }

    return cache[n];
}
