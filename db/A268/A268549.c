/*
 * A268549  Diagonal of (1 - 9 x y)/((1 - 3 y - 2 x + 3 y^2 + 9 x^2 y) * (1 -
 *          u - z) * (1 - v - w)).
 * https://oeis.org/A268549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*(3*n-2)*(2*n-1)^2 * a(n-1) / n^3 Initial value: a(0) =
 *            1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

/* cache of computed terms */
static seq_t *cache = NULL;       /* array of sequence values */
static size_t cached = 0;          /* number of terms currently cached (length) */

/* compute a(n) for arbitrary n, memoising results */
seq_t a(idx_t n)
{
    /* no need for values with negative index; n >= 0 */
    if (n >= cached) {
        /* extend cache to n+1 terms */
        size_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* initialise first elements if this is the first call */
        if (cached == 0) {
            cache[0] = (seq_t)1;
            cached = 1;
        }
        /* compute missing terms using the recurrence */
        for (idx_t i = cached; i <= n; ++i) {
            seq_t val = cache[i - 1];
            /* val = val * 12 / i */
            val = mul_small(val, (unsigned int)12);
            val = val / (seq_t)i;
            /* val = val * (3*i - 2) / i */
            val = mul_small(val, (unsigned int)(3 * i - 2));
            val = val / (seq_t)i;
            /* val = val * (2*i - 1) / i */
            val = mul_small(val, (unsigned int)(2 * i - 1));
            val = val / (seq_t)i;
            /* val = val * (2*i - 1) */
            val = mul_small(val, (unsigned int)(2 * i - 1));
            cache[i] = val;
        }
        cached = new_size;
    }
    return cache[n];
}
