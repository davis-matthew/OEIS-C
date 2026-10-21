/*
 * A281185  a(0)=0, a(1)=1, a(2)=0; thereafter, a(2n) = a(n) + a(n+1) for n >=
 *          2, a(2n+1) = a(n) for n >= 1.
 * https://oeis.org/A281185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=0; for n>=2: a(2n) = a(n) + a(n+1); for
 *            n>=1: a(2n+1) = a(n)
 * Range:     u8 n <= 9787, u16 n <= 9787, u32 n <= 9787, u64 n <= 9787,
 *            u128 n <= 9787, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest n whose value is cached */

    /* If nothing cached yet, allocate and fill the base cases */
    if (cache == NULL) {
        cache = seq_alloc(n + 1);
        if (n >= 0) cache[0] = 0;
        if (n >= 1) cache[1] = 1;
        if (n >= 2) cache[2] = 0;
        max_computed = (n >= 2) ? 2 : n;
    }

    /* If a larger index is requested, grow the cache and compute new values */
    if (n > max_computed) {
        cache = seq_realloc(cache, max_computed + 1, n + 1);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 0) {
                /* already set in initial allocation */
            } else if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 0;
            } else if ((i & 1) == 0) {        /* i is even */
                idx_t k = i / 2;
                cache[i] = cache[k] + cache[k + 1];
            } else {                          /* i is odd */
                idx_t k = (i - 1) / 2;
                cache[i] = cache[k];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
