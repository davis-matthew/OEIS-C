/*
 * A387877  a(n) = Sum_{k=0..n} binomial(5*n+1,6*k).
 * https://oeis.org/A387877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+1,6*k). Recurrent: a(0)=1,
 *            a(1)=2, a(2)=463, a(3)=9829, a(4)=349525, a(n) = 6*a(n-1) +
 *            615*a(n-2) + 7160*a(n-3) - 7155*a(n-4) + 7776*a(n-5) for n > 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL; /* cached values */
static idx_t  cache_len  = 0;    /* number of elements in cache */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Base values a(0) .. a(4) */
static const seq_t base_vals[5] = {
    (seq_t)1, (seq_t)2, (seq_t)463, (seq_t)9829, (seq_t)349525
};

seq_t a(idx_t n) {
    /* Initialize cache with five base terms if not yet done */
    if (cache == NULL) {
        cache_cap = 5;
        cache_len = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; return 0 as fallback */
        for (idx_t i = 0; i < 5; ++i) cache[i] = base_vals[i];
    }

    /* If requested index already cached, return directly */
    if (n + 1 <= cache_len) {
        return cache[n];
    }

    /* Ensure capacity large enough to hold up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        if (cache_cap < new_cap * 2) {
            new_cap = cache_cap * 2;
            if (new_cap < n + 1) new_cap = n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* reallocation failed; return 0 as fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 5) {
            /* Should not occur since cache_len starts at 5 */
            cache[i] = base_vals[i];
            continue;
        }
        seq_t val = cache[i-1] * (seq_t)6
                  + cache[i-2] * (seq_t)615
                  + cache[i-3] * (seq_t)7160
                  + cache[i-5] * (seq_t)7776;
        /* Subtract the term with negative coefficient */
        val -= cache[i-4] * (seq_t)7155;
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
