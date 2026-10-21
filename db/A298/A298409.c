/*
 * A298409  a(n) = 2*a(n-1) - a(n-3) + 2*a(floor(n/2)) + 3*a(floor(n/3)) + ...
 *          + n*a(floor(n/n)), where a(0) = 1, a(1) = 2, a(2) = 3.
 * https://oeis.org/A298409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) – a(n-3) + Σ_{k=2..n} k * a(⌊n/k⌋) Initial
 *            values: a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 36, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t  next_to_compute = 0;/* next index that has not been computed */

    /* initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 3) cache_cap = 3;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* OOM */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        next_to_compute = 3;                  /* 0,1,2 are ready */
    }

    /* grow the cache if the requested index is beyond its size */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 3) new_cap = 3;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        if (i < 3) continue;                /* base cases already set */

        /* Σ_{k=2..i} k * a(⌊i/k⌋) via grouping of equal quotients */
        seq_t sum = 0;
        for (idx_t l = 2; l <= i; ) {
            idx_t q = i / l;
            idx_t r = i / q;                 /* last k with the same q */
            if (r > i) r = i;                /* safety */

            seq_t l_seq = (seq_t)l;
            seq_t r_seq = (seq_t)r;
            seq_t cnt_seq = (seq_t)(r - l + 1);
            seq_t k_sum = ((l_seq + r_seq) * cnt_seq) / 2;

            sum += k_sum * cache[(idx_t)q];
            l = r + 1;
        }

        /* add the remaining recurrence terms */
        sum += (seq_t)2 * cache[i - 1];
        sum -= cache[i - 3];

        cache[i] = sum;
    }

    if (n >= next_to_compute) next_to_compute = n + 1;
    return cache[n];
}
