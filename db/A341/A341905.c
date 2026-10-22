/*
 * A341905  a(n) = a(n-1) + a(n-2) + 2*a(n-3) with a(0) = 3, a(1) = 0, a(2) =
 *          2.
 * https://oeis.org/A341905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3), a(0) = 3, a(1) = 0, a(2) = 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache (memoisation) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache            */
    static size_t used  = 0;   /* number of entries that are defined */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap  = 8;                     /* start with small buffer           */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();   /* OOM – just terminate              */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        used = 3;
    }

    /* grow cache if it cannot contain index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();     /* OOM */
        cache = tmp;
        cap   = new_cap;
    }

    /* fill missing values up to n */
    if (used <= (size_t)n) {
        for (size_t i = used; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)2 * cache[i-3];
        }
        used = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
