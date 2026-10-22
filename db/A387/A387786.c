/*
 * A387786  a(0)=2, a(1)=1, and thereafter a(n) = R(a(n-1) + a(n-2)) where
 *          R(x) = A004086(x) is the decimal digit reversal of x.
 * https://oeis.org/A387786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=2, a(1)=1; a(n)=rev_decimal(a(n-1)+a(n-2)) for n>=2
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 32, u64 n <= 66, u128 n <= 125,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Lazy memoization cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size */
    static size_t cache_size = 0; /* number of computed terms */

    /* Ensure base cases are present */
    if (capacity < 2) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        capacity = 2;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    if (n < cache_size)
        return cache[(size_t)n];

    /* Grow cache if necessary */
    if (capacity <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t sum = cache[(size_t)(k - 1)] + cache[(size_t)(k - 2)];
        cache[(size_t)k] = reverse_decimal(sum);
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
