/*
 * A384429  Consecutive states of the linear congruential pseudo-random number
 *          generator for Prime Sheffield Pascal when started at 1.
 * https://oeis.org/A384429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16807 * a(n-1) mod 2^31 , a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to allow arbitrary call order */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* allocated elements */
static idx_t cache_max = 0;     /* highest n computed (>=1 if cache not empty) */

seq_t a(idx_t n)
{
    /* Ensure n >= 1 */
    if (n == 0) return 0; /* undefined for 0 */
    /* Allocate initial cache if empty */
    if (cache_max == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;   /* a(1) = 1 */
        cache_max = 1;
        if (!cache) return 0;   /* allocation failed */
    }
    /* Grow cache if needed */
    if (n > cache_max) {
        idx_t new_cap = cache_cap;
        if (new_cap < n) {
            /* double until enough */
            while (new_cap < n)
                new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* Compute missing values */
        for (idx_t i = cache_max; i < n; ++i) {
            unsigned long long prev = (unsigned long long)cache[i-1];
            /* 16807 * prev modulo 2^31, lower 31 bits */
            unsigned long long cur = (prev * 16807ULL) & 0x7FFFFFFFULL;
            cache[i] = (seq_t)cur;
        }
        cache_max = n;
    }
    return cache[n-1];
}
