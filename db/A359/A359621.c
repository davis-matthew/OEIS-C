/*
 * A359621  Number of edge cuts in the n-prism graph.
 * https://oeis.org/A359621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 146*a(n-2) + 488*a(n-3) - 777*a(n-4) * +
 *            612*a(n-5) - 228*a(n-6) + 32*a(n-7) * Initial values: a(0)=1,
 *            a(1)=4, a(2)=31, a(3)=314, a(4)=3013, * a(5)=27060, a(6)=232671
 *            * This function implements the recurrence with memoisation. * It
 *            works in both C11 and C++17 builds
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;     /* Memoisation buffer */
static idx_t   cache_len = 0;   /* Number of stored elements */
static size_t  cache_cap  = 0;   /* Allocated capacity               */

seq_t a(idx_t n)
{
    /* Initialise buffer with the first 7 terms if empty */
    if (cache_len == 0) {
        size_t init_cap = 7;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        if (!cache) abort();          /* allocation failed */

        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 31;
        cache[3] = 314;
        cache[4] = 3013;
        cache[5] = 27060;
        cache[6] = 232671;

        cache_cap  = init_cap;
        cache_len  = 7;
    }

    /* Extend buffer up to index n if required */
    if (n >= cache_len) {
        /* Grow to at least n+1 */
        size_t new_cap = cache_cap ? cache_cap : 7;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        if (new_cap > cache_cap) {
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            for (size_t i = 0; i < cache_cap; ++i)
                tmp[i] = cache[i];
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* realloc failed */
            cache = tmp;
#endif
            cache_cap = new_cap;
        }

        /* Compute missing values via the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i >= 7) {
                cache[i] = 20 * cache[i-1]
                         - 146 * cache[i-2]
                         + 488 * cache[i-3]
                         - 777 * cache[i-4]
                         + 612 * cache[i-5]
                         - 228 * cache[i-6]
                         + 32  * cache[i-7];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
