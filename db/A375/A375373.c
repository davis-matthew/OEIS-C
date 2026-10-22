/*
 * A375373  Expansion of 1/( (1 + x)^2 * (1 - x^2*(1 + x)^2) ).
 * https://oeis.org/A375373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-3) + 6*a(n-4) + 4*a(n-5) + a(n-6)
 *            Initial values: a(0)=1, a(1)=-2, a(2)=4, a(3)=-4, a(4)=6,
 *            a(5)=-4
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 48, i64 n <= 94, i128 n <= 187,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache to avoid repeated allocations */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure capacity for n+1 elements */
    if ((size_t)(n + 1) > cap) {
        seq_t *new_cache = seq_alloc_new_array((size_t)(n + 1));
        if (cache) seq_free_array(cache);
        cache = new_cache;
        cap   = (size_t)(n + 1);
    }

    /* Base cases */
    cache[0] = 1;
    if (n >= 1) cache[1] = -2;
    if (n >= 2) cache[2] = 4;
    if (n >= 3) cache[3] = -4;
    if (n >= 4) cache[4] = 6;
    if (n >= 5) cache[5] = -4;

    /* Recurrence for n >= 6 */
    for (idx_t i = 6; i <= n; ++i) {
        cache[i] = -2 * cache[i - 1]
                   + 4 * cache[i - 3]
                   + 6 * cache[i - 4]
                   + 4 * cache[i - 5]
                   + 1 * cache[i - 6];
    }

    return cache[n];
}
