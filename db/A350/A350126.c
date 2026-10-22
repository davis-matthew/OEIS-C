/*
 * A350126  a(n) = (a(a(n-1)) mod 2) + a(n-2) with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A350126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(a(n-1)) mod 2) + a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of initialized entries      */
    static size_t cap   = 0;   /* current allocated capacity         */

    /* enlarge the cache if needed */
    if (len <= (size_t)n) {
        size_t new_cap = cap ? cap : 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;               /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base values if not yet done */
    if (len == 0) {
        cache[0] = 0;
        len = 1;
        if (cap > 1) {
            cache[1] = 1;
            len = 2;
        }
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1) continue;     /* already initialised */

        seq_t prev = cache[i-1];
        idx_t  inner_index = (idx_t)prev;     /* a(n-1) is an earlier index */
        seq_t inner_val   = cache[inner_index];
        seq_t val         = (inner_val & 1) + cache[i-2];
        cache[i] = val;
    }

    len = (size_t)n + 1;
    return cache[n];
}
