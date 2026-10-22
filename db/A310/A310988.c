/*
 * A310988  Coordination sequence Gal.4.16.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = 2*a(n-7) - a(n-14)   for n > 14
 * Initial values (n = 0 … 14):
 *   1, 4, 8, 10, 14, 16, 23, 24, 31, 32, 34, 38, 40, 50, 48
 */

#include <stdlib.h>   /* for malloc / realloc */
#include <stddef.h>   /* for size_t */

static const seq_t INIT[] = {
    1, 4, 8, 10, 14, 16, 23, 24, 31, 32, 34, 38, 40, 50, 48
};
static seq_t *CACHE = NULL;     /* memoized values */
static idx_t  CACHE_LEN = 0;    /* length of CACHE (number of elements computed) */

static void ensure_cache(idx_t n)
{
    if (n < CACHE_LEN) return;              /* already present */

    idx_t old_len = CACHE_LEN;
    CACHE_LEN = n + 1;
    CACHE = (seq_t *)realloc(CACHE, CACHE_LEN * sizeof(seq_t));

    for (idx_t i = old_len; i < CACHE_LEN; ++i) {
        if (i < (idx_t)(sizeof(INIT) / sizeof(INIT[0]))) {
            CACHE[i] = INIT[i];
        } else {
            /* a(i) = 2*a(i-7) - a(i-14) for i >= 15 */
            CACHE[i] = (seq_t)2 * CACHE[i - 7] - CACHE[i - 14];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return CACHE[n];
}
