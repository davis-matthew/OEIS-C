/*
 * A256601  Numbers n such that the decimal expansions of both n and n^2 have
 *          1 as smallest digit and 9 as largest digit.
 * https://oeis.org/A256601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 1584, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has1and9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A256601: numbers n such that the decimal expansions of both n and n^2
 * have 1 as the smallest digit and 9 as the largest digit.
 * The function builds the sequence term by term, caching found values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has1and9.h"

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined for offset 0 */

    /* Static cache for previously found terms. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;           /* number of stored terms   */
    static size_t cache_cap = 0;           /* capacity of cache array  */
    static seq_t  candidate  = 1;          /* next integer to test     */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Test current candidate. */
        if (has1and9(candidate) && has1and9(candidate * candidate)) {
            /* Ensure sufficient capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Allocation failed – abort. */
                    exit(EXIT_FAILURE);
                }
                /* Copy existing terms. */
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                /* Free old cache if it exists. */
                if (cache) free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            /* Store the found term. */
            cache[cache_len++] = candidate;
        }
        /* Advance to the next integer. */
        ++candidate;
        /* If candidate wrapped around (overflow of seq_t), stop searching.
           The sequence may be larger than the type can represent; in that
           case further terms cannot be represented and the cache will not
           grow to the requested size. */
        if (candidate == 0) break;
    }

    /* If we couldn't find enough terms (due to overflow), return 0. */
    if (cache_len < n) return 0;
    return cache[n - 1];
}
