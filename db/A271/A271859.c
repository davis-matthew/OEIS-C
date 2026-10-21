/*
 * A271859  Six steps forward, five steps back.
 * https://oeis.org/A271859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12), a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=5, a(8)=4, a(9)=3, a(10)=2,
 *            a(11)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initial 12 terms */
    static const seq_t init_vals[12] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4,
        (seq_t)5, (seq_t)6, (seq_t)5, (seq_t)4, (seq_t)3,
        (seq_t)2, (seq_t)1
    };

    if (n < 12)
        return init_vals[(size_t)n];

    /* initialise cache if not done yet */
    if (cache_len == 0) {
        size_t cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return init_vals[0]; /* fallback if malloc fails */
        cache_cap = cap;
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_len = 12;
    }

    /* ensure capacity for index n */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_len = cache_cap = 0;
            /* recompute without caching (unlikely for n <= 10000) */
            seq_t *tmp2 = (seq_t *)malloc(need * sizeof(seq_t));
            if (!tmp2) return init_vals[0];
            for (size_t i = 0; i < 12; ++i) tmp2[i] = init_vals[i];
            for (size_t i = 12; i < need; ++i)
                tmp2[i] = tmp2[i-1] + tmp2[i-11] - tmp2[i-12];
            seq_t r = tmp2[n];
            free(tmp2);
            return r;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];

    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
