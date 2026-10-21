/*
 * A274306  a(n) = Product_{k=1..n} (4*k^4+1).
 * https://oeis.org/A274306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (4*k^4+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of stored terms */

    /* initialise cache[0] = 1 on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        cache_cap = 1;
    }

    /* if value already computed, return it */
    if ((size_t)n < cache_cap)
        return cache[n];

    /* enlarge cache if necessary */
    size_t old_cap = cache_cap;
    if (cache_cap <= (size_t)n) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute missing terms from old_cap to n */
    for (idx_t i = old_cap; i <= n; ++i) {
        seq_t k = (seq_t)i;
        seq_t k2 = k * k;            /* k^2 */
        seq_t k4 = k2 * k2;          /* k^4 */
        seq_t term = 4 * k4 + 1;     /* 4*k^4 + 1 */
        seq_t prev = (i == 0) ? (seq_t)1 : cache[i - 1];
        cache[i] = prev * term;
    }

    return cache[n];
}
