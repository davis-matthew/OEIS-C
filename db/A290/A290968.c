/*
 * A290968  a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) + a(n-5), with
 *          a(0)=a(1)=a(2)=1, a(3)=-1 and a(4)=1.
 * https://oeis.org/A290968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) + a(n-5) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=-1, a(4)=1
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 48, i64 n <= 94, i128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the nth term of OEIS sequence A290968. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;          /* allocated capacity */
    static idx_t  computed = 0;           /* number of terms stored (next free index) */

    /* Handle indices less than 5 directly. */
    if (n <= 4) {
        switch (n) {
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return -1;
            case 4: return 1;
            default: break; /* unreachable */
        }
    }

    /* Allocate cache on first use. */
    if (cache == NULL) {
        /* Ensure capacity at least n+1, but start with a reasonable minimal size. */
        idx_t init_cap = n + 1;
        if (init_cap < 16) init_cap = 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, fallback to zero */

        /* Initialize the first five values. */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = -1;
        cache[4] = 1;

        computed = 5;           /* indices 0..4 have been stored */
        cache_cap = init_cap;
    }
    else if (n + 1 > cache_cap) {
        /* Grow the cache to accommodate up to index n. */
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have computed up to n, just return. */
    if (n < computed) {
        return cache[n];
    }

    /* Compute new terms up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4] + cache[i-5];
    }
    computed = n + 1;

    return cache[n];
}
