/*
 * A247698  Brady numbers: B(n) = B(n - 1) + B(n - 2) with B(1) = 2308 and
 *          B(2) = 4261.
 * https://oeis.org/A247698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 2308, a(2) = 4261
 * Range:     u8 none, u16 n <= 7, u32 n <= 30, u64 n <= 76, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity (highest index that can be accessed) */
    static idx_t last  = 0;   /* highest index computed so far */

    /* Ensure cache is allocated and initialized with the two seed values */
    if (cache == NULL) {
        cap = 4;                    /* accommodate indices 0..3 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;/* allocation failed – return zero */
        cache[0] = (seq_t)0;        /* unused placeholder */
        cache[1] = (seq_t)2308;
        cache[2] = (seq_t)4261;
        last = 2;
    }

    /* Grow cache capacity if needed */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) {
            new_cap = new_cap * 2;
        }
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;/* allocation failed */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing terms up to n if necessary */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        last = n;
    }

    return cache[n];
}
