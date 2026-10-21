/*
 * A297339  Number of 2 X n 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 2*a(n-3) - a(n-5) (n >= 6) Base
 *            cases: a(1)=4, a(2)=8, a(3)=22, a(4)=53, a(5)=134
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset: unspecified behaviour allowed. */
    if (n == 0) return 0;

    /* Direct return for the first five values. */
    if (n <= 5) {
        switch (n) {
        case 1: return (seq_t)4;
        case 2: return (seq_t)8;
        case 3: return (seq_t)22;
        case 4: return (seq_t)53;
        case 5: return (seq_t)134;
        default: return 0; /* unreachable */
        }
    }

    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Allocate or resize the cache as necessary. */
    size_t required = (size_t)n + 1; /* indices 0..n inclusive */

    if (cache == NULL) {
        cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure: return 0 */
        cache_len = required;

        /* initialise first entries (index 0 unused for this sequence) */
        cache[0] = 0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)53;
        cache[5] = (seq_t)134;

        /* compute the sequence up to n */
        for (size_t i = 6; i <= n; ++i) {
            cache[i] = cache[i - 1]
                       + (seq_t)3 * cache[i - 2]
                       + (seq_t)2 * cache[i - 3]
                       - cache[i - 5];
        }
        return cache[n];
    }

    if (required > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        /* compute new values beyond the previous cache length */
        for (size_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1]
                       + (seq_t)3 * cache[i - 2]
                       + (seq_t)2 * cache[i - 3]
                       - cache[i - 5];
        }
        cache_len = required;
    }

    return cache[n];
}
