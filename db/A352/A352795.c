/*
 * A352795  Number of ways to tile a 4 X n strip with squares and L-shaped
 *          heptominoes with legs of equal length.
 * https://oeis.org/A352795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-4) + 2*a(n-5) + 2*a(n-6) + 2*a(n-7), a(0)
 *            = 1, a(1) = 1, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache: cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* next entry to compute (first unfilled index) */
    static idx_t cap   = 0;   /* allocated capacity */

    /* Ensure enough storage for indices 0 … n. */
    if (n >= cap) {
        idx_t new_cap = (n + 1 > cap * 2) ? (n + 1) : (cap * 2);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) {
            /* Allocation failure – return 0 (caller may treat as error). */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        if (i <= 3) {
            /* Initial values. */
            val = 1;
        } else {
            /* Recurrence; guard against negative indices. */
            val = cache[i - 1];
            if (i >= 4) val += 4 * cache[i - 4];
            if (i >= 5) val += 2 * cache[i - 5];
            if (i >= 6) val += 2 * cache[i - 6];
            if (i >= 7) val += 2 * cache[i - 7];
        }
        cache[i] = val;
    }

    size = n + 1;   /* All 0 … n are now computed. */
    return cache[n];
}
