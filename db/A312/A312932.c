/*
 * A312932  Coordination sequence Gal.5.81.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 * Initial values: *
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, * a(5)=22, a(6)=27,
 *            a(7)=31, a(8)=36, a(9)=40, a(10)=44 * (the recurrence uses the
 *            above initial terms)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed values; dynamic growth is required for
 * arbitrary indices and for arbitrary-precision seq_t.  The cache
 * holds values a(0) .. a(cache_capacity-1).  It grows geometrically
 * to avoid frequent reallocations.  Use calloc so that newly added
 * entries are zero-initialised; the recurrence guarantees they
 * will be overwritten before use. */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;

/* Initial terms of the sequence – 11 values correspond to n = 0..10 */
static const seq_t base_vals[11] = {
    1, 4, 9, 13, 18, 22, 27, 31, 36, 40, 44
};

/* Ensure that the cache is large enough to contain a(n).  When
 * enlarging, already stored values are preserved and new entries
 * are filled using the recurrence (for n > 10) or the fixed
 * initial values (for n <= 10). */
static void ensure_capacity(idx_t n)
{
    if (n < cache_capacity)
        return;

    /* Grow capacity: double it, or enough for n, whichever is larger */
    idx_t new_capacity = cache_capacity == 0 ? 1 : 2 * cache_capacity;
    if (new_capacity <= n)
        new_capacity = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure – leave cache unchanged;
         * caller will receive a zero value. */
        return;
    }
    cache = new_cache;

    /* Fill new entries */
    for (idx_t i = cache_capacity; i < new_capacity; ++i) {
        if (i <= 10) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
        }
    }
    cache_capacity = new_capacity;
}

/* Public function required by the challenge */
seq_t a(idx_t n)
{
    /* Compute any missing terms up to n */
    ensure_capacity(n);

    /* If allocation failed, return 0 as a safe default */
    if (n >= cache_capacity)
        return 0;

    return cache[n];
}
