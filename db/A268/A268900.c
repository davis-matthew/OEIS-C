/*
 * A268900  Number of n X 4 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) – 81*a(n-2) for n>3, with a(1)=36, a(2)=696,
 *            a(3)=9720 (derived from a(n)=8*3^(2n-3)*(16n-3) for n>1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;      /* 1‑indexed: cache[1] = a(1) */
    static idx_t cache_len = 0;      /* highest n for which cache[i] is set */

    /* determine size needed (at least up to 3 to guard the recurrence) */
    idx_t needed = n;
    if (needed < 3)
        needed = 3;
    /* resize cache if required */
    if (cache_len == 0) {
        cache = (seq_t*)realloc(cache, (needed + 1) * sizeof(seq_t));
        cache_len = 0;
    } else if (needed + 1 > cache_len + 1) {
        cache = (seq_t*)realloc(cache, (needed + 1) * sizeof(seq_t));
    }

    /* initialise base cases if not already done */
    if (cache_len < 1) {
        cache[0] = 0;               /* unused */
        cache[1] = (seq_t)36;
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[2] = (seq_t)696;
        cache_len = 2;
    }
    if (cache_len < 3) {
        cache[3] = (seq_t)9720;
        cache_len = 3;
    }

    /* extend cache up to requested n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 3) {
            cache[i] = (seq_t)9720;
        } else {
            cache[i] = (seq_t)18 * cache[i - 1] - (seq_t)81 * cache[i - 2];
        }
    }
    cache_len = n;

    return cache[n];
}
