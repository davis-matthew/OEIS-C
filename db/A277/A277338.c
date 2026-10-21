/*
 * A277338  Reverse and Add! sequence starting with 295.
 * https://oeis.org/A277338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 295; a(n) = a(n-1) + reverse_decimal(a(n-1)) for n ≥ 1
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first term on first call */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)295;
        cache_len = 1;
    }

    /* if the requested term is already cached */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* compute terms up to n inclusive */
    size_t max_n = (size_t)n;
    for (size_t i = cache_len; i <= max_n; ++i) {
        /* grow cache if necessary */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[i] = prev + rev;
    }
    cache_len = max_n + 1;
    return cache[max_n];
}
