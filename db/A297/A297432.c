/*
 * A297432  Number of 2 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + 4*a(n-4) (n >= 5) Base values: a(1) =
 *            1, a(2) = 2, a(3) = 7, a(4) = 12
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) {               /* undefined offset */
        return (seq_t)0;
    }

#ifndef __cplusplus
    /* --------------------- C implementation --------------------- */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of values stored (in terms of n) */

    if (n > cached) {
        /* Grow the cache to hold terms up to n */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {              /* allocation failure */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached; i < n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)7;
            } else if (i == 3) {
                cache[3] = (seq_t)12;
            } else {
                cache[i] = cache[i - 1] + cache[i - 3] +
                           (seq_t)4 * cache[i - 4];
            }
        }
        cached = n;
    }

    return cache[n - 1];
#else
    /* --------------------- C++ implementation --------------------- */
    static std::vector<seq_t> cache;

    if (cache.size() < n) {
        cache.resize(n);   /* grow the vector */

        for (idx_t i = cache.size() == n ? 0 : cache.size(); i < n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)7;
            } else if (i == 3) {
                cache[3] = (seq_t)12;
            } else {
                cache[i] = cache[i - 1] + cache[i - 3] +
                           (seq_t)4 * cache[i - 4];
            }
        }
    }

    return cache[n - 1];
#endif
}
