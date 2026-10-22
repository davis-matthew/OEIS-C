/*
 * A307799  a(0) = 0, a(1) = 3; a(n) = rev(a(n-1))*a(n-1) + a(n-2), where rev
 *          = digit reversal (A004086).
 * https://oeis.org/A307799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(1) = 3; recurrence: a(n) = rev(a(n-1)) * a(n-1) +
 *            a(n-2), where rev(x) is the decimal digit reversal of x
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   reverse_decimal_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard library headers */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_decimal_local.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* pointer to the cached values   */
    static size_t cache_cap = 0;    /* allocated capacity (size of array) */
    static size_t last_idx  = 1;    /* largest index that is already known */

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cache_cap = 2;                     /* need indices 0 and 1 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;/* allocation failure – unlikely in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
    }

    /* return immediately if the value is already cached */
    if (n <= last_idx) return cache[(size_t)n];

    /* grow the cache until it can hold index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(last_idx + 1); i <= n; ++i) {
        seq_t rev_prev = reverse_decimal_local(cache[(size_t)(i - 1)]);
        cache[(size_t)i] = rev_prev * cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
    }
    last_idx = n;        /* update the largest known index */

    return cache[(size_t)n];
}
