/*
 * A399354  Number of independent vertex sets in the 3 X n Moebius strip
 *          graph.
 * https://oeis.org/A399354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 8*a(n-2) + 6*a(n-3) - a(n-4) - a(n-5), initial
 *            values: a(0)=3, a(1)=3, a(2)=15, a(3)=45, a(4)=179, a(5)=623
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;       /* number of elements allocated */
static idx_t cache_max = (idx_t)(-1); /* largest index computed (-1 means none) */

/* Pre‑computed initial terms. */
static const seq_t init_vals[6] = {3, 3, 15, 45, 179, 623};

seq_t a(idx_t n)
{
    /* Ensure the cache can hold at least n+1 elements. */
    if (cache == NULL || (size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 6) new_cap = 6;          /* at least space for the 6 initials */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0.  (Should not happen in normal use.) */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first six values if they have not been done yet. */
    if (cache_max == (idx_t)(-1)) {
        for (idx_t i = 0; i < 6 && i < (idx_t)cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_max = 5;   /* last initial value index */
    }

    /* If the requested index is already known, just return it. */
    if (n <= cache_max)
        return cache[n];

    /* Compute new values up to n, extending the cache as needed. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* All operations are performed in the natural type seq_t. */
        seq_t val = cache[i-1];
        val += (seq_t)8 * cache[i-2];
        val += (seq_t)6 * cache[i-3];
        val -= cache[i-4];
        val -= cache[i-5];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
