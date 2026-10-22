/*
 * A393926  a(0) = 1; a(n) = 3 - 2*n + Sum_{k=0..n-1} a(k) * a(n-1-k).
 * https://oeis.org/A393926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3 - 2*n + Sum_{k=0..n-1} a(k) * a(n-1-k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t target = (size_t)n;

    /* Grow the cache to at least n+1 entries */
    if (cache_len <= target) {
        size_t new_len = target + 1;
        seq_t *tmp = NULL;

#ifdef __cplusplus
        /* In C++ use new[] which calls constructors for class types */
        tmp = new seq_t[new_len];
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            delete[] cache;
        }
#else
        /* In C use realloc (void* to seq_t* cast needed in C++) */
        tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            exit(1);
#endif
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k)
                sum += cache[k] * cache[i - 1 - k];
            seq_t val = sum + (seq_t)3;
            val -= (seq_t)2 * (seq_t)i;
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
