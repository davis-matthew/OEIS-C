/*
 * A312722  Coordination sequence Gal.5.146.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 10 with initial values a(0)=1
 *            , a(1)=4 , a(2)=8 , a(3)=15 , a(4)=18 , a(5)=24 , a(6)=26 ,
 *            a(7)=31 , a(8)=38 , a(9)=40 , a(10)=48 The function is memoized
 *            to answer calls in any order
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* array of a(0)..a(max_n) */
    static size_t  cached_len = 0;  /* number of values stored (>= 11) */
    static size_t  cache_cap  = 0;  /* allocated capacity */

    /* pre‑computed first 11 terms */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)4,  (seq_t)8,  (seq_t)15, (seq_t)18,
        (seq_t)24, (seq_t)26, (seq_t)31, (seq_t)38, (seq_t)40,
        (seq_t)48
    };

    /* return directly if n is one of the base values */
    if (n <= 10) {
        return init[(size_t)n];
    }

    /* first call: allocate cache and copy base values */
    if (!cache) {
        cache_cap = 64;                    /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;              /* allocation failed */
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
        cached_len = 11;
    }

    /* make sure the cache can hold index n */
    size_t nn = (size_t)n;
    if (cache_cap <= nn) {
        size_t new_cap = cache_cap;
        while (new_cap <= nn) { new_cap <<= 1; } /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (size_t i = cached_len; i <= nn; ++i) {
        seq_t v = cache[i - 5];
        v += v;                /* 2*a(i-5) */
        v -= cache[i - 10];    /* -a(i-10) */
        cache[i] = v;
    }
    cached_len = nn + 1;

    return cache[nn];
}
