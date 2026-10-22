/*
 * A304129  Number of nX4 0..1 arrays with every element unequal to 0, 1, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-3) +a(n-5) -a(n-6) -5*a(n-7) -2*a(n-8)
 *            +2*a(n-10) for n>13, with initial values a(1)=5, a(2)=17,
 *            a(3)=19, a(4)=44, a(5)=80, a(6)=140, a(7)=244, a(8)=436,
 *            a(9)=800, a(10)=1444, a(11)=2584, a(12)=4630, a(13)=8328
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 73, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not a valid index, return 0 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;

    /* initial values (1-indexed) */
    static const seq_t init[13] = {
        5, 17, 19, 44, 80, 140, 244, 436, 800, 1444, 2584, 4630, 8328
    };

    /* enlarge cache if needed */
    if (cache_size < (size_t)n) {
        size_t new_capacity = (size_t)n;
        if (cache_capacity < new_capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure: return 0 */
            cache = tmp;
            cache_capacity = new_capacity;
        }
        /* compute missing terms */
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            if (i < 13) {
                cache[i] = init[i];
            } else {
                /* term number = i+1 */
                size_t idx = i + 1;
                seq_t val = cache[idx-2]                /* a(idx-1) */
                         + (seq_t)3 * cache[idx-4]      /* 3*a(idx-3) */
                         + cache[idx-6]                 /*   a(idx-5) */
                         - cache[idx-7]                 /* - a(idx-6) */
                         - (seq_t)5 * cache[idx-8]      /* -5*a(idx-7) */
                         - (seq_t)2 * cache[idx-9]      /* -2*a(idx-8) */
                         + (seq_t)2 * cache[idx-11];    /* +2*a(idx-10) */
                cache[i] = val;
            }
        }
        cache_size = (size_t)n;
    }
    return cache[n-1];
}
