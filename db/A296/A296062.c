/*
 * A296062  Base-2 logarithm of the number of different shapes of balanced
 *          binary trees with n nodes (A110316).
 * https://oeis.org/A296062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 596, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula for A296062
 *
 * a(0) = 0, a(1) = 0
 * a(2*n)   = a(n)   + a(n-1) + 1
 * a(2*n+1) = 2 * a(n)
 *
 * The function computes values iteratively and caches them so that
 * any order of calls is efficient.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache  = NULL;   /* array of seq_t values */
    static size_t  cap    = 0;     /* allocated capacity (>= number of slots) */
    static size_t  min_n  = 0;     /* next index that needs computation */

    /* Ensure capacity for indices 0..n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – undefined result */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = min_n; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 0;
        } else if ((i & 1) == 0) {          /* even i = 2*k */
            size_t k = i >> 1;
            cache[i] = cache[k] + cache[k - 1] + 1;
        } else {                            /* odd i = 2*k + 1 */
            size_t k = i >> 1;
            cache[i] = 2 * cache[k];
        }
    }
    min_n = need;

    return cache[(size_t)n];
}
