/*
 * A310521  Coordination sequence Gal.5.133.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) - a(n-3) + 2*a(n-4)
   - a(n-5) - a(n-6) + 2*a(n-7) - a(n-8)
   with a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=22,
   a(5)=26, a(6)=32, a(7)=38, a(8)=44 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values */
static size_t cache_len = 0;       /* number of elements stored */

/* Base values for n = 0..8 */
static const seq_t base_vals[9] = {
    1, 4, 10, 16, 22, 26, 32, 38, 44
};

seq_t a(idx_t n) {
    /* Handle small n directly */
    if (n < 9) return base_vals[(size_t)n];

    /* Initialise cache with base values if not already done */
    if (cache_len == 0) {
        cache_len = 9;
        /* malloc returns void*, cast needed for C++ compat */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = base_vals[i];
    }

    size_t req_len = (size_t)n + 1;
    if (req_len > cache_len) {
        /* Extend cache */
        seq_t *new_cache = (seq_t*)realloc(cache, req_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;

        /* Compute new values using the recurrence */
        for (size_t i = cache_len; i < req_len; ++i) {
            cache[i] = 2 * cache[i-1]
                       - cache[i-2] - cache[i-3]
                       + 2 * cache[i-4]
                       - cache[i-5] - cache[i-6]
                       + 2 * cache[i-7]
                       - cache[i-8];
        }
        cache_len = req_len;
    }

    return cache[(size_t)n];
}
