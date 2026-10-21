/*
 * A268881  Number of n X 3 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 31*a(n-2) + 30*a(n-3) - 9*a(n-4) (n ≥ 5) with
 *            a(1)=2, a(2)=14, a(3)=84, a(4)=462
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(count) -> seq_t* */

static const seq_t base_vals[5] = {
    0ULL,          /* unused index 0 */
    2ULL,          /* a(1) */
    14ULL,         /* a(2) */
    84ULL,         /* a(3) */
    462ULL         /* a(4) */
};

static seq_t *cache = NULL;      /* cache[0] unused, cache[1..] holds a(n) */
static size_t cache_cap = 0;     /* maximum index currently cached */

/* Return a(n) for any n ≥ 1.  The function may be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, not used by callers */

    if (n <= 4)                      /* small indices served directly */
        return base_vals[(size_t)n];

    /* Ensure the cache exists and contains base values up to 4. */
    if (cache_cap < 4) {
        const size_t init_cap = 4;
        cache = alloc_seq_array(init_cap + 1);   /* indices 0..4 */
        /* zero‑initialize the array (alloc_seq_array should already do this) */
        for (size_t i = 0; i <= init_cap; ++i) cache[i] = 0;
        for (size_t i = 1; i <= init_cap; ++i) cache[i] = base_vals[i];
        cache_cap = init_cap;
    }

    /* Expand the cache if the requested index is beyond the current capacity. */
    if ((size_t)n > cache_cap) {
        const size_t new_cap = (size_t)n;
        seq_t *new_cache = alloc_seq_array(new_cap + 1); /* 0 .. new_cap */
        /* copy old values; cache is guaranteed to be non‑NULL here */
        for (size_t i = 0; i <= cache_cap; ++i)
            new_cache[i] = cache[i];

        /* Compute new terms using the recurrence.  All indices are valid. */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
            new_cache[i] =
                new_cache[i-1] * 10
                - new_cache[i-2] * 31
                + new_cache[i-3] * 30
                - new_cache[i-4] * 9;
        }

        /* Release old cache (optional; replacement keeps cache persistent). */
        /* free(cache);  // free() is sufficient for malloc‑based alloc_seq_array */

        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
