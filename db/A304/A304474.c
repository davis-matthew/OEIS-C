/*
 * A304474  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 6*a(n-3) + 8*a(n-4) + 5*a(n-5) - 6*a(n-6) -
 *            22*a(n-7) - 25*a(n-8) - 14*a(n-9) + 2*a(n-10) + 7*a(n-11) +
 *            3*a(n-12) for n>13 Initial values: a(1)=4, a(2)=12, a(3)=19,
 *            a(4)=37, a(5)=94, a(6)=202, a(7)=428, a(8)=965, a(9)=2134,
 *            a(10)=4692, a(11)=10385, a(12)=22939, a(13)=50685 (Offset...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[14] = {
    0,      /* dummy for a(0) */
    4, 12, 19, 37, 94, 202, 428, 965, 2134, 4692, 10385, 22939, 50685
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for invalid indices */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    /* resize cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        if (new_size < 16) new_size = 16;          /* avoid tiny allocations */
        if (cache == NULL) {
            cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc((void *)cache,
                                      (size_t)new_size * sizeof(seq_t));
        }
        cache_size = new_size;
    }

    /* fill missing values up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = init_vals[i];
            } else {
                cache[i] =
                    cache[i - 2]
                    + 6 * cache[i - 3]
                    + 8 * cache[i - 4]
                    + 5 * cache[i - 5]
                    - 6 * cache[i - 6]
                    - 22 * cache[i - 7]
                    - 25 * cache[i - 8]
                    - 14 * cache[i - 9]
                    + 2 * cache[i - 10]
                    + 7 * cache[i - 11]
                    + 3 * cache[i - 12];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
