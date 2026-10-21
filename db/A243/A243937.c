/*
 * A243937  Even numbers n>=6 for which lpf(n-1) > lpf(n-3), where lpf = least
 *          prime factor.
 * https://oeis.org/A243937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth even n ≥ 6 with lpf(n-1) > lpf(n-3)
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence values        */
    static idx_t cache_len = 0;          /* number of stored terms         */
    static size_t cache_cap = 0;         /* current capacity of cache      */
    static idx_t next_candidate = 6;     /* next even integer to test      */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t cand = next_candidate;     /* current even candidate         */
        next_candidate += 2;             /* step to the next even number   */

        if (lpf(cand - 1) > lpf(cand - 3)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    if (cache) free(cache);
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
