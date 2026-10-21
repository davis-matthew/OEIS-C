/*
 * A295851  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = -1, a(2) = 2, a(3) = 1.
 * https://oeis.org/A295851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4); Initial values:
 *            a(0)=0, a(1)=-1, a(2)=2, a(3)=1
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 42, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#if !defined(__cplusplus)
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;            /* array of computed terms */
    static size_t cache_cap = 0;           /* allocated capacity */
    static idx_t  cache_len = 0;           /* number of terms stored (last index + 1) */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len  = 4;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* enlarge cache as needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 4;
        while (new_cap <= (size_t)n + 1) {
            new_cap <<= 1;
        }
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) {
            tmp[i] = cache[i];
        }
        delete[] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1) + 3*a(i-2) - 2*a(i-3) - 2*a(i-4) */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = t1 + (seq_t)3 * t2 - (seq_t)2 * t3 - (seq_t)2 * t4;
    }

    cache_len = (idx_t)(n + 1);
    return cache[(size_t)n];
}
