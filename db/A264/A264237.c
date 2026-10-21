/*
 * A264237  Sum of values of vertices at level n of the hyperbolic Pascal
 *          pyramid.
 * https://oeis.org/A264237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 99*a(n-2) + 226*a(n-3) - 224*a(n-4) +
 *            92*a(n-5) - 12*a(n-6) (for n >= 7), with initial values a(0) =
 *            1, a(1) = 3, a(2) = 9, a(3) = 33, a(4) = 165, a(5) = 1137, a(6)
 *            = 9837
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t _a_init[7] = {
    (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)33,
    (seq_t)165, (seq_t)1137, (seq_t)9837
};

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t  capacity = 0;     /* number of allocated slots */
    static size_t  max_comp  = 0;     /* last computed index   */

    /* Ensure cache can hold indices up to n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* very small possibility of failure */

        /* If we previously had no data, initialise the first 7 terms */
        if (max_comp == 0) {
            for (size_t i = 0; i < 7 && i < new_cap; ++i)
                new_cache[i] = _a_init[i];
            max_comp = 6;
        }

        cache = new_cache;
        capacity = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= max_comp)
        return cache[(size_t)n];

    /* Compute missing terms iteratively */
    for (size_t i = max_comp + 1; i <= (size_t)n; ++i) {
        seq_t t1 = mul_small(cache[i-1], 18);
        seq_t t2 = mul_small(cache[i-2], 99);
        seq_t t3 = mul_small(cache[i-3], 226);
        seq_t t4 = mul_small(cache[i-4], 224);
        seq_t t5 = mul_small(cache[i-5], 92);
        seq_t t6 = mul_small(cache[i-6], 12);

        seq_t val = t1;
        val -= t2;
        val += t3;
        val -= t4;
        val += t5;
        val -= t6;

        cache[i] = val;
    }

    max_comp = n;
    return cache[(size_t)n];
}
