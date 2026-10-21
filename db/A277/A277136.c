/*
 * A277136  Numbers k such that cos(k) > 0 and cos(k+2) > 0.
 * https://oeis.org/A277136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A277136: numbers k such that cos(k) > 0 and cos(k+2) > 0
   by brute‑force search using double arithmetic (cos from <math.h>). */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: test whether cos(k)>0 && cos(k+2)>0 using double. */
static inline bool good_k(idx_t k)
{
    return cos((double)k) > 0.0 && cos((double)(k + 2)) > 0.0;
}

/* The main function.  It caches previously found terms so that calls in any
   order are fast.  The cache is grown on demand. */
seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;          /* array of found ks */
    static size_t capacity = 0;          /* allocated size of cache */
    static idx_t count = 0;              /* number of stored terms */
    static idx_t last_k = 1;             /* next k to examine */

    /* Return cached value if already available */
    if (n <= count)
        return cache[n - 1];

    /* Grow cache if necessary */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Extend the cache until we have at least n terms */
    for (/* nothing */; count < n; ++last_k) {
        if (good_k(last_k)) {
            if (count == capacity) {
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[count++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
