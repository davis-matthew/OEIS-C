/*
 * A295726  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = -1, a(2) = 1, a(3) = 1.
 * https://oeis.org/A295726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 42, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295726:
 * Recurrence: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4)
 * Initial values: a(0)=0, a(1)=-1, a(2)=1, a(3)=1
 * Indexing starts at n=0.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of values currently stored (max index + 1) */
    static idx_t cache_cap = 0;  /* capacity of the allocated array */

    /* Initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = (n > 4 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* out of memory: undefined behaviour */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* Grow the cached array if needed */
    if (n >= cache_len) {
        if (n + 1 > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap <= n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)3 * cache[i-2]
                       - (seq_t)2 * cache[i-3] - (seq_t)2 * cache[i-4];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
