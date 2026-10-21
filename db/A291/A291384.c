/*
 * A291384  a(n) = (1/2)*A291383(n).
 * https://oeis.org/A291384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 4*a(n-3) + 2*a(n-4) * Initial
 *            values: a(0)=1, a(1)=4, a(2)=14, a(3)=49, a(4)=172 * Offset: 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A291384 */
seq_t a(idx_t n)
{
    /* Initial values for the recurrence (a(0) … a(4)) */
    static const seq_t init[5] = { (seq_t)1,
                                   (seq_t)4,
                                   (seq_t)14,
                                   (seq_t)49,
                                   (seq_t)172 };

    /* Cache of already computed values */
    static seq_t *cache = NULL;    /* pointer to cached terms */
    static size_t cache_len = 0;   /* number of terms cached */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Return directly for indices less than 5 */
    if (n < 5) {
        return init[(size_t)n];
    }

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 64;                      /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {                 /* allocation failure */
            /* In production code you might handle this error differently */
            return (seq_t)0;
        }
        for (size_t i = 0; i < 5; ++i) {
            cache[i] = init[i];
        }
        cache_len = 5;
    }

    /* Ensure we have computed terms up to n */
    if ((size_t)n >= cache_len) {
        size_t desired = (size_t)n + 1;       /* +1 because zero-based */

        /* Resize cache if necessary */
        if (desired > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < desired) {
                new_cap = desired * 2;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {                 /* allocation failure */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (size_t i = cache_len; i < desired; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)2;
            seq_t t2 = cache[i - 2] * (seq_t)4;
            seq_t t3 = cache[i - 3] * (seq_t)4;
            seq_t t4 = cache[i - 4] * (seq_t)2;
            cache[i] = t1 + t2 + t3 + t4;
        }
        cache_len = desired;
    }

    return cache[(size_t)n];
}
