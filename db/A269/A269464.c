/*
 * A269464  Number of length-n 0..5 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 10*a(n-2) - 50*a(n-3). Initial values: a(1)=6,
 *            a(2)=36, a(3)=210. Indexing starts at 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    if (n < 1)
        return (seq_t)0;            /* out‑of‑range values are undefined */

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first three values if they are not yet present */
    if (cache_len == 0) {
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)210;
        cache_len = 3;
    } else if (cache_len == 1) {
        cache[1] = (seq_t)36;
        cache_len = 2;
    } else if (cache_len == 2) {
        cache[2] = (seq_t)210;
        cache_len = 3;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t v = (seq_t)9  * cache[i-1]
                - (seq_t)10 * cache[i-2]
                - (seq_t)50 * cache[i-3];
        cache[i] = v;
    }
    cache_len = n;

    return cache[n-1];
}
