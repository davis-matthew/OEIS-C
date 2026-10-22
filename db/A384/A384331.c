/*
 * A384331  Consecutive internal states of a linear congruential pseudo-random
 *          number generator for Microsoft C and C++ when started at 1.
 * https://oeis.org/A384331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (214013 * a(n-1) + 2531011) mod 2^31, * a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned long long MODULO = 2147483648ULL; /* 2^31 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n<1 */

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;                  /* number of values stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;              /* allocation failure */
        cache[0] = (seq_t)1;                      /* a(1) */
    }

    /* grow cache if needed */
    if ((idx_t)cache_len < n) {
        size_t required = (size_t)n;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            if (new_cap < required) new_cap = required;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t prev = cache[i-1];
            unsigned long long prod =
                214013ULL * (unsigned long long)prev + 2531011ULL;
            cache[i] = (seq_t)(prod % MODULO);      /* a(i+1) */
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
