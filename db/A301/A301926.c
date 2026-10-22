/*
 * A301926  a(n+3) = a(n) + 24*n + 32, a(0)=0, a(1)=3, a(2)=13.
 * https://oeis.org/A301926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = a(n) + 24*n + 32, a(0)=0, a(1)=3, a(2)=13
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Compute the OEIS sequence A301926.
 * Uses a minimal static cache to allow calls in arbitrary order.
 * Works for any build that defines seq_t and idx_t.
 */
seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure we have at least the first three values initialised */
    if (cache_len < 3) {
        /* grow to 3 if needed */
        if (cache_len == 0) {
            cache_len = 3;
            cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
            if (!cache) return 0; /* allocation failure: return 0 as fallback */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)3;
            cache[2] = (seq_t)13;
        } else {
            /* bump to 3 if still smaller (should not happen) */
            cache = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            if (!cache) return 0;
            if (cache_len == 1) cache[1] = (seq_t)3;
            if (cache_len == 2) cache[2] = (seq_t)13;
            cache_len = 3;
        }
    }

    /* if already computed, just return the value */
    if (n < cache_len) {
        return cache[n];
    }

    /* resize cache to hold up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3) continue; /* initial terms already set */
        /* a(i) = a(i-3) + 24*(i-3) + 32 */
        seq_t term = cache[i-3]
                     + ((seq_t)24 * (seq_t)(i-3))
                     + (seq_t)32;
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n];
}
