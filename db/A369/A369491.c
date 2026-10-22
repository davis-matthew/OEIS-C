/*
 * A369491  a(n) = n! * [x^n] (2*x - 4*exp(x) + 3*exp(2*x) + 3) / 2.
 * https://oeis.org/A369491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2), a(0)=1, a(1)=2, a(2)=4, a(3)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..len] holds values up to a(len) */
static size_t cache_cap = 0;  /* allocated size of cache */
static size_t cache_len = 0;  /* highest index currently computed */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)4;
            case 3: return (seq_t)10;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* initialize the cache on first use */
    if (cache == NULL) {
        cache_cap = 4;                 /* store up to index 3 initially */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)10;
        cache_len = 3;
    }

    /* extend the cache if needed */
    if ((idx_t)n > (idx_t)cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;            /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;

        /* compute missing values using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            cache[i] = (seq_t)(3 * prev1 - 2 * prev2);
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
