/*
 * A394272  Number of induced cubes in the n-Menger sponge graph.
 * https://oeis.org/A394272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1)-244*a(n-2)+480*a(n-3) Initial values: a(1) = 1,
 *            a(2) = 44, a(3) = 1264
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based) */
    static seq_t *cache = NULL;       /* cache[1]..cache[cache_len] are computed */
    static size_t cache_len = 0;      /* highest index that has been computed */
    static size_t cache_cap = 0;      /* current allocated capacity (index limit) */

    /* initialise on first call */
    if (cache == NULL) {
        /* capacity for indices 0..3 (indices 0..3 inclusive) */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */

        cache[0] = 0;                       /* unused placeholder */
        cache[1] = 1;                       /* a(1) */
        cache[2] = 44;                      /* a(2) */
        cache[3] = 1264;                    /* a(3) */
        cache_len = 3;
    }

    /* convert index to size_t for array use – n ≥ 1 by contract */
    size_t idx = (size_t)n;

    /* grow the cache until idx is available */
    while (cache_len < idx) {
        /* ensure sufficient capacity for the next element */
        if (cache_len + 1 >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= cache_len + 1) new_cap = cache_len + 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence: a(n) = 31*a(n-1)-244*a(n-2)+480*a(n-3) */
        seq_t t1 = cache[cache_len]   * 31;   /* a(n-1) */
        seq_t t2 = cache[cache_len-1] * 244;  /* a(n-2) */
        seq_t t3 = cache[cache_len-2] * 480;  /* a(n-3) */
        seq_t next = t1 - t2 + t3;

        cache[++cache_len] = next;
    }

    return cache[idx];
}
