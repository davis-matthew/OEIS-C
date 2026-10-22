/*
 * A302678  Number of nX6 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3) + a(n-4) - 2*a(n-5) - a(n-6)
 *            for n>12 Initial values: a(1) = 13, a(2) = 13, a(3) = 40, a(4) =
 *            76, a(5) = 187, a(6) = 453, a(7) = 1125, a(8) = 2782, a(9) =
 *            6843, a(10) = 16908, a(11) = 41758, a(12) = 103027
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* sequence initial values (index 0 unused) */
static const seq_t init[13] = {
    0,
    13, 13, 40, 76, 187,
    453, 1125, 2782, 6843,
    16908, 41758, 103027
};

/* cache of computed terms, indexed from 0 to cached */
static seq_t *cache = NULL;
static idx_t cached = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 12) {
        return init[n];
    }

    /* allocate cache if not yet done */
    if (!cache) {
        size_t sz = 13;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        for (idx_t i = 1; i <= 12; ++i) {
            cache[i] = init[i];
        }
        cache[0] = 0;
        cached = 12;
    }

    /* expand cache and compute missing terms */
    if ((size_t)n > cached) {
        size_t new_sz = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));

        for (size_t i = (size_t)(cached + 1); i <= (size_t)n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                seq_t pos = cache[i - 1];
                pos += cache[i - 2] * 2;
                pos += cache[i - 3] * 4;
                pos += cache[i - 4];

                seq_t neg = cache[i - 5] * 2 + cache[i - 6];
                cache[i] = pos - neg;
            }
        }
        cached = n;
    }

    return cache[(size_t)n];
}
