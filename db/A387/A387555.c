/*
 * A387555  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 2^(n-k) *
 *          binomial(2*k+2,2*n-6*k+1).
 * https://oeis.org/A387555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-3) + 16*a(n-4) - 16*a(n-6) + 64*a(n-7) - 64*a(n-8)
 *            Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=8, a(4)=16, a(5)=0,
 *            a(6)=48, a(7)=320
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* base values for n < 8 */
    static const seq_t base[8] = {
        (seq_t)1, (seq_t)0, (seq_t)0,
        (seq_t)8, (seq_t)16, (seq_t)0,
        (seq_t)48, (seq_t)320
    };

    if (n < 8) return base[n];

    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t last_computed = 0;  /* largest index computed */

    /* lazy initialization of cache */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) cache[i] = base[i];
        last_computed = 7;
    }

    /* extend cache if needed and compute missing values */
    if ((idx_t)n > last_computed) {
        /* grow capacity */
        if ((idx_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) return (seq_t)0; /* allocation failure */
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* compute values from last_computed+1 up to n */
        for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
            seq_t pos = 8 * cache[i-3] + 16 * cache[i-4] + 64 * cache[i-7];
            seq_t neg = 16 * cache[i-6] + 64 * cache[i-8];
            cache[i] = pos - neg;
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
