/*
 * A302644  a(n) =
 *          (n+2)*(n+1)*(n^6-12*n^5+70*n^4-210*n^3+409*n^2-378*n+360)/720.
 * https://oeis.org/A302644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n+2)*(n+1)*(n^6-12*n^5+70*n^4-210*n^3+409*n^2-378*n+360)/720
 *            Recurrence for n > 8: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) -
 *            126*a(n-4) + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) +
 *            a(n-9) Initial values: a(0)=1, a(1)=2, a(2)=6, a(3)=20, a(4)=70,
 *            a(5)=252, a(6)=896, a(7)=2976, a(8)=8955
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 37, u64 n <= 583,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* next index to be filled */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure the cache can hold up to index n */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < need) new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* initial values (a(0) ... a(8)) */
        static const seq_t init_vals[9] = {
            (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)20, (seq_t)70,
            (seq_t)252, (seq_t)896, (seq_t)2976, (seq_t)8955
        };
        for (size_t i = cache_len; i < need; ++i) {
            if (i <= 8) {
                cache[i] = init_vals[i];
            } else {
                cache[i] =
                    (seq_t)9 *  cache[i-1]
                  - (seq_t)36 * cache[i-2]
                  + (seq_t)84 * cache[i-3]
                  - (seq_t)126 * cache[i-4]
                  + (seq_t)126 * cache[i-5]
                  - (seq_t)84 * cache[i-6]
                  + (seq_t)36 * cache[i-7]
                  - (seq_t)9 *  cache[i-8]
                  +          cache[i-9];
            }
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
