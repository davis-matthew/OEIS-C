/*
 * A280599  Number of n X 3 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 3*a(n-3) + 2*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) Initial values: a(1)=4, a(2)=18, a(3)=45, a(4)=116,
 *            a(5)=293, a(6)=735, a(7)=1844, a(8)=4626
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term; guard against n==0 which is outside the offset */
    if (n == 0) return 0;

    /* static cache of computed terms; cache[i] = a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* grow cache (using realloc cast to seq_t*) if needed */
    if (n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed */
        cache = tmp;

        /* initial eight values (0‑based indices 0..7 correspond to a(1)..a(8)) */
        static const seq_t init[8] = {4, 18, 45, 116, 293, 735, 1844, 4626};

        for (idx_t i = cache_size; i < n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* recurrence for n > 8 (i >= 8) */
                cache[i] =
                    4 * cache[i-1]      /* 4*a(n-1) */
                  - 3 * cache[i-2]      /* -3*a(n-2) */
                  - 3 * cache[i-3]      /* -3*a(n-3) */
                  + 2 * cache[i-4]      /* +2*a(n-4) */
                  + 3 * cache[i-5]      /* +3*a(n-5) */
                  - 2 * cache[i-6];     /* -2*a(n-6) */
            }
        }
        cache_size = n;
    }

    return cache[n-1];
}
