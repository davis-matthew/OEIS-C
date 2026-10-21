/*
 * A250793  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) – 20*a(n-2) + 8*a(n-3) + 33*a(n-4) – 36*a(n-5) +
 *            8*a(n-7) for n ≥ 8; a(1) … a(7) are given below
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recurrence for A250793 */
seq_t a(idx_t n)
{
    /* Pre‑computed initial values */
    static const seq_t init[8] = {
        0,      /* 0 unused, index starts at 1 */
        118,    /* a(1) */
        376,    /* a(2) */
        1190,   /* a(3) */
        3776,   /* a(4) */
        12062,  /* a(5) */
        38676,  /* a(6) */
        124366  /* a(7) */
    };

    /* Static cache storage */
    static seq_t *cache = NULL;    /* 1‑based array */
    static size_t cache_cap = 0;   /* allocated size (without dummy 0 index) */
    static idx_t cache_len = 0;    /* greatest n for which cache[n] is valid */

    /* Extend cache if necessary */
    if (n > cache_len) {
        /* Ensure capacity */
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed – return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing values */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = init[i];
            } else {
                cache[i] =
                    (seq_t)8  * cache[i - 1]
                  - (seq_t)20 * cache[i - 2]
                  + (seq_t)8  * cache[i - 3]
                  + (seq_t)33 * cache[i - 4]
                  - (seq_t)36 * cache[i - 5]
                  + (seq_t)8  * cache[i - 7];
            }
        }
        cache_len = n;
    }
    return cache[n];
}
