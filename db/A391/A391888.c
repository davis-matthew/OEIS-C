/*
 * A391888  a(0)=5 and a(n) = n*(a(n-1) + 2) for n > 0.
 * https://oeis.org/A391888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; a(n) = n * (a(n-1) + 2) (n > 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache to allow arbitrary call order and reuse */
    static seq_t *cache = NULL;       /* array of computed a(k) */
    static idx_t cache_cap = 0;       /* number of elements stored (size of cache) */

    /* initialize cache for n = 0 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0; /* allocation failure → zero value */
        cache[0] = (seq_t)5;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0; /* allocation failure → zero value */
        cache = tmp;

        /* compute new values using the recurrence */
        for (idx_t i = cache_cap; i <= n; ++i) {
            cache[i] = (seq_t)i * (cache[i - 1] + (seq_t)2);
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
