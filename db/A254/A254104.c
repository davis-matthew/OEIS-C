/*
 * A254104  Permutation of natural numbers: a(0) = 0, a(3n) = 1 + 2*a(2n - 1),
 *          a(3n+1) = 1 + 2*a(2n), a(3n+2) = 2*a(n+1).
 * https://oeis.org/A254104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(3n) = 1 + 2*a(2n-1); a(3n+1) = 1 + 2*a(2n); a(3n+2)
 *            = 2*a(n+1);
 * Range:     u8 n <= 69, u16 n <= 1847, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A254104. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache can hold a[n] */
    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;

#ifdef __cplusplus
        seq_t *new_cache = static_cast<seq_t *>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        /* In case realloc failed (unlikely for normal use), keep old cache */
        if (!new_cache)
            return (seq_t)0;
        /* Initialise new entries to 0 (sentinel for “not yet computed”) */
        for (size_t i = old_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if it exists (value 0 is only legitimate for n==0) */
    if ((size_t)n < cache_cap && (n == 0 || cache[n] != (seq_t)0))
        return cache[n];

    /* Base case */
    if (n == (idx_t)0) {
        cache[0] = (seq_t)0;
        return (seq_t)0;
    }

    seq_t result;
    idx_t k = n / 3;            /* k = floor(n/3) */

    if ((n % 3) == 0) {          /* a(3k) = 1 + 2*a(2k-1) */
        seq_t t = a((idx_t)(2 * k - 1));
        result = t + t + (seq_t)1;   /* 1 + 2*t */
    } else if ((n % 3) == 1) {   /* a(3k+1) = 1 + 2*a(2k) */
        seq_t t = a((idx_t)(2 * k));
        result = t + t + (seq_t)1;
    } else {                     /* a(3k+2) = 2*a(k+1) */
        seq_t t = a(k + (idx_t)1);
        result = t + t;
    }

    cache[n] = result;
    return result;
}
