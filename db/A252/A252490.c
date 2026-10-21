/*
 * A252490  Numbers whose set of digits is simply connected, with 9 and 0
 *          considered as neighbors.
 * https://oeis.org/A252490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose set of digits is a single
 *            contiguous block on the cyclic digit set 0–9 (with 9 and 0
 *            considered neighbors)
 * Range:     u8 n <= 62, u16 n <= 3691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contiguous_mask_valid, ensure_capacity, number_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/contiguous_mask_valid.h"
#include "../utils/number_is_valid.h"

/* Main OEIS sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* not defined for n=0, return 0 as placeholder */

    static seq_t *cache      = NULL;         /* cached sequence values */
    static size_t cache_len = 0;             /* number of cached values */
    static size_t cache_cap = 0;             /* allocated capacity of cache array */
    static idx_t  next_num   = 1;            /* next natural number to test */

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Generate new values until we have at least n terms. */
    while (cache_len < (size_t)n) {
        idx_t val = next_num++;
        if (number_is_valid(val)) {
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_len++] = (seq_t)val;
        }
    }
    return cache[n - 1];
}
