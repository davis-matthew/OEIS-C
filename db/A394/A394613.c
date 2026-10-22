/*
 * A394613  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 4, a(2) = 16,
 *          a(3) = 20.
 * https://oeis.org/A394613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4); a(0)=1, a(1)=4, a(2)=16,
 *            a(3)=20
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc */

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;       /* pointer to array of seq_t */
    static size_t cache_cap = 0;     /* number of entries currently cached */

    /* Fast return if value already known. */
    if (n < (idx_t)cache_cap)
        return cache[(size_t)n];

    /* Ensure the cache can hold indices up to n. */
    size_t need   = (size_t)n + 1;    /* number of elements required */
    size_t old_cap = cache_cap;       /* number of elements already computed */

    if (need > old_cap) {
        seq_t *tmp = seq_realloc(cache, old_cap, need);
        if (!tmp)                     /* allocation failure – unlikely for n <= 10000 */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = need;
    }

    /* Compute any newly required terms.  The base cases are handled explicitly. */
    for (size_t i = old_cap; i < need; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)16;
        else if (i == 3)
            cache[i] = (seq_t)20;
        else
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }

    return cache[(size_t)n];
}
