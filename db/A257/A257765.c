/*
 * A257765  Positive integers whose square is the sum of 26 consecutive
 *          squares.
 * https://oeis.org/A257765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*a(n-3) - a(n-6) Initial values: a(1)=195, a(2)=1599,
 *            a(3)=2379, a(4)=19695, a(5)=163059, a(6)=242619
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;
    static size_t cap = 0;

    /* Initial terms (index 0 unused) */
    static const seq_t init[7] = {0,
        (seq_t)195,  (seq_t)1599,  (seq_t)2379,
        (seq_t)19695, (seq_t)163059, (seq_t)242619};

    if (n == 0) return 0;

    /* Allocate cache for the first call */
    if (cache == NULL) {
        /* We need indices 0..6, so allocate 6+1 entries */
        cap = 6;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
        max_cached = 6;
    }

    /* Extend cache if necessary */
    if ((size_t)n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap   = (size_t)n;
        }
    }

    /* Compute any missing terms up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = init[i];
            } else {
                seq_t temp = mul_small(cache[i - 3], 102u);
                temp -= cache[i - 6];
                cache[i] = temp;
            }
        }
        max_cached = n;
    }

    return cache[n];
}
