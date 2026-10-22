/*
 * A313543  Coordination sequence Gal.6.196.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = a(n-1) + d(n) (n ≥ 1) * d(1) = 4 * for n > 1
 *            : d(n) = {5,5,5,4,5,4,6,4,5,4,5}[ n mod 11 ] (period 11)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   diff_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/diff_at.h"

/* Cached values of the coordination sequence. */
static seq_t *cache      = NULL;   /* array holding a(0) … a(cached_len-1)   */
static idx_t  cached_len = 0;      /* number of stored elements              */
static size_t cached_cap = 0;      /* allocated capacity of the array        */

seq_t a(idx_t n)
{
    /* Expand the cache if necessary. */
    if (n + 1 > cached_cap) {
        size_t need = (size_t)(n + 1);
        size_t new_cap = cached_cap ? cached_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the program. */
            abort();
        }
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Compute missing values up to index n. */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else
            cache[i] = cache[i - 1] + diff_at(i);
    }
    cached_len = n + 1;
    return cache[n];
}
