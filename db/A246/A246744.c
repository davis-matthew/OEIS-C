/*
 * A246744  Number of length 7+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a linear recurrence of order 21: a(n) = 3*a(n-1)
 *            +6*a(n-2) -26*a(n-3) -9*a(n-4) +99*a(n-5) -24*a(n-6) -216*a(n-7)
 *            +126*a(n-8) +294*a(n-9) -252*a(n-10) -252*a(n-11) +294*a(n-12)
 *            +126*a(n-13) -216*a(n-14) -24*a(n-15) +99*a(n-16) -9*a(n-17)
 *            -26*a(n-18) +6*a(n-19) +3*a(n-20) -a(n-21) w...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   count_initial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_initial.h"

static const int32_t coeff[21] = {
    3, 6, -26, -9, 99, -24, -216, 126, 294, -252, -252,
    294, 126, -216, -24, 99, -9, -26, 6, 3, -1
};

/* Memoised sequence a(n) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* indices 1.. */
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            if (cache) free(cache);
            cache = NULL;
            cache_cap = 0;
            return (seq_t)0;
        }
        if (tmp != cache) cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute sequentially up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 21) {
            cache[i] = count_initial(i);
        } else {
            __int128 sum = 0;
            for (int k = 1; k <= 21; ++k) {
                sum += coeff[k - 1] * ( (__int128)cache[i - k] );
            }
            cache[i] = (seq_t)sum;
        }
        max_computed = i;
    }

    return cache[n];
}
