/*
 * A398246  Number of feedback vertex sets in the grid graph P_n X P_3.
 * https://oeis.org/A398246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 3*a(n-2) - 9*a(n-3) + 10*a(n-4); Initial
 *            values: a(0)=1, a(1)=8, a(2)=57, a(3)=416
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds already-computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current allocated capacity */
    static size_t cache_size = 0;  /* number of terms stored (>= cache_size may be larger) */

    size_t idx = (size_t)n;   /* safe cast for indexing */

    /* ensure that cache is initialised and large enough to hold a[idx] */
    if (cache_size == 0) {
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed; unlikely in tests */
        cache_cap = init_cap;
        /* initialise the first four terms */
        cache[0] = (seq_t)1u;
        cache[1] = (seq_t)8u;
        cache[2] = (seq_t)57u;
        cache[3] = (seq_t)416u;
        cache_size = 4;
    }

    /* grow cache if needed */
    if (idx >= cache_size) {
        size_t needed = idx + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (size_t i = cache_size; i <= idx; ++i) {
            /* recurrence: a(i) = 7*a(i-1) + 3*a(i-2) - 9*a(i-3) + 10*a(i-4) */
            seq_t term = (seq_t)7 * cache[i-1]
                       + (seq_t)3 * cache[i-2]
                       - (seq_t)9 * cache[i-3]
                       + (seq_t)10 * cache[i-4];
            cache[i] = term;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
