/*
 * A284523  a(1) = a(2) = 1, a(3) = 2; a(n) = a(a(n-1)-1) + a(n-a(n-3)-1) for
 *          n > 3.
 * https://oeis.org/A284523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = a(2) = 1, a(3) = 2; a(n) = a(a(n-1)-1) + a(n-a(n-3)-1)
 *            for n > 3
 * Range:     u8 n <= 653, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache ----------------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static bool initialized = false;

    /* ensure the cache is large enough for the current request */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap <= (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory error */
        /* initialise new entries to zero (sentinel for "uncomputed") */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* initialise base cases only once */
    if (!initialized) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        initialized = true;
    }

    /* if already computed, return immediately */
    if (cache[n] != (seq_t)0) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;      /* already computed */

        /* indices for the recurrence */
        idx_t j1 = (idx_t)cache[i-1] - 1;           /* a(a(i-1)-1)  */
        idx_t j2 = (idx_t)i - (idx_t)cache[i-3] - 1;/* a(i-a(i-3)-1) */

        /* safety: ensure indices are within bounds (should be true) */
        if (j1 >= cap) {                           /* extend if needed */
            size_t newcap = cap ? cap * 2 : 8;
            while (newcap <= (size_t)j1 + 1) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            for (size_t k = cap; k < newcap; ++k) tmp[k] = (seq_t)0;
            cache = tmp;
            cap = newcap;
        }
        if (j2 >= cap) {
            size_t newcap = cap ? cap * 2 : 8;
            while (newcap <= (size_t)j2 + 1) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            for (size_t k = cap; k < newcap; ++k) tmp[k] = (seq_t)0;
            cache = tmp;
            cap = newcap;
        }

        cache[i] = cache[j1] + cache[j2];
    }

    return cache[n];
}
