/*
 * A252935  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n>12;
 *            a(1)=15, a(2)=83, a(3)=494, a(4)=3067, a(5)=17962, a(6)=86488,
 *            a(7)=320270, a(8)=917811, a(9)=2127013, a(10)=4211511,
 *            a(11)=7437417, a(12)=12070971
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[12] = {
    15, 83, 494, 3067, 17962, 86488, 320270, 917811,
    2127013, 4211511, 7437417, 12070971
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range call */

    static seq_t *cache  = NULL;   /* cache of computed values a(1)..a(max) */
    static size_t cap    = 0;      /* allocated capacity of cache       */
    static idx_t  max_n  = 0;      /* number of values in cache         */

    /* allocate/expand cache if necessary */
    if ((size_t)n > cap) {
        size_t newcap = (cap * 2 > (size_t)n) ? cap * 2 : n;
        if (newcap == 0) newcap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i - 1] = init[i - 1];
            } else {
                seq_t val  = 4 * cache[i - 2];   /* 4*a(i-1) */
                val -= 6 * cache[i - 3];         /* -6*a(i-2) */
                val += 4 * cache[i - 4];         /* +4*a(i-3) */
                val -=      cache[i - 5];        /* -a(i-4)  */
                cache[i - 1] = val;
            }
        }
        max_n = n;
    }

    return cache[n - 1];
}
