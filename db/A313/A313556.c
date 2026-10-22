/*
 * A313556  Coordination sequence Gal.6.132.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10 a(0)=1, a(1)=5, a(2)=10, a(3)=14, a(4)=19,
 *            a(5)=24, a(6)=28, a(7)=32, a(8)=37, a(9)=42, a(10)=46
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_size = 0;     /* number of elements currently cached */

/* compute a(n) using the recurrence and memoisation */
seq_t a(idx_t n)
{
    /* initialise cache with the first 11 terms */
    if (!cache) {
        cache = (seq_t*)malloc(11 * sizeof(seq_t));
        if (!cache) { /* allocation failed – should not happen */ }
        const seq_t init[11] = {1, 5, 10, 14, 19, 24, 28, 32, 37, 42, 46};
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_size = 11;
    }

    /* extend cache to cover n, computing new entries on the fly */
    if (n + 1 > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – should not happen */ }
        cache = tmp;

        /* compute missing terms from the first unfound index onward */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 11) {
                /* these indices are already initialised, but keep the code clear */
                /* cache[i] = init[i]; not needed because they were set earlier */
            } else {
                /* recurrence: a(i) = a(i-1)+a(i-3)+a(i-7)+a(i-9) - (a(i-4)+a(i-6)+a(i-10)) */
                seq_t s = cache[i - 1] + cache[i - 3] + cache[i - 7] + cache[i - 9];
                seq_t t = cache[i - 4] + cache[i - 6] + cache[i - 10];
                cache[i] = s - t;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
