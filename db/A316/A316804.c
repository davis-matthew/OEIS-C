/*
 * A316804  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -5*a(n-2) +50*a(n-3) -18*a(n-4) -81*a(n-5)
 *            +4*a(n-6) +20*a(n-7) initial values: a(1)=8, a(2)=128,
 *            a(3)=1905, a(4)=28248, a(5)=420345, a(6)=6256258, a(7)=93109949
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Cache to store already computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t filled = 0;   /* highest n for which cache[n-1] is valid */

    /* Initialize cache with first seven terms if needed */
    if (!cache) {
        cache = (seq_t*)malloc(7 * sizeof(seq_t));
        cache[0] = 8;
        cache[1] = 128;
        cache[2] = 1905;
        cache[3] = 28248;
        cache[4] = 420345;
        cache[5] = 6256258;
        cache[6] = 93109949;
        filled = 7;
    }

    /* Return term if already computed */
    if (n <= filled) {
        return cache[n-1];
    }

    /* Grow cache if necessary */
    if (n > filled) {
        cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        for (idx_t k = filled + 1; k <= n; ++k) {
            /* Compute a(k) using the 7‑term recurrence */
            seq_t sum = mul_small(cache[k-2], 15)                          /* 15*a(k-1) */
                    + mul_small(cache[k-4], 50)                            /* 50*a(k-3) */
                    + mul_small(cache[k-7], 4)                             /* 4*a(k-6) */
                    + mul_small(cache[k-8], 20);                          /* 20*a(k-7) */

            seq_t sub = mul_small(cache[k-3], 5)                          /* 5*a(k-2) */
                    + mul_small(cache[k-5], 18)                           /* 18*a(k-4) */
                    + mul_small(cache[k-6], 81);                          /* 81*a(k-5) */

            cache[k-1] = sum - sub;                                      /* final a(k) */
        }
        filled = n;
    }
    return cache[n-1];
}
