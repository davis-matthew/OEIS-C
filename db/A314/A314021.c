/*
 * A314021  Coordination sequence Gal.5.84.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 with initial values:
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=15, a(4)=20, a(5)=24, a(6)=29,
 *            a(7)=33, a(8)=39, a(9)=44, a(10)=49
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[11] = {
    1ULL, 5ULL, 11ULL, 15ULL, 20ULL,
    24ULL, 29ULL, 33ULL, 39ULL, 44ULL, 49ULL
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    /* For arguments below 11 just return the hard‑coded values */
    if (n < 11) return base_values[n];

    /* Grow the cache so that it contains entries up to n */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Initialise missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 11) {
                cache[i] = base_values[i];
            } else {
                /* a(i) = a(i-1) + a(i-9) - a(i-10) */
                seq_t temp = cache[i - 1] + cache[i - 9];
                cache[i] = temp - cache[i - 10];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
