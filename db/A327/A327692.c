/*
 * A327692  Number of length-n phone numbers that can be dialed by a chess
 *          knight on a 0-9 keypad that starts on any number and takes n-1
 *          steps.
 * https://oeis.org/A327692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - 4*a(n-4) for n > 6; initial values: a(1)=10,
 *            a(2)=20, a(3)=46, a(4)=104, a(5)=240, a(6)=544
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Return the first six values directly */
    if (n <= 6) {
        static const seq_t base[7] = {0,10,20,46,104,240,544};
        return base[n];
    }

    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of the cache (number of slots) */
    static idx_t cache_len = 0;   /* highest index that has been computed */

    /* initialise cache the first time */
    if (cache_len == 0) {
        cache_cap = 7;                   /* slots 0..6 (use index 0 as dummy) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[1] = 10;  cache[2] = 20;   cache[3] = 46;
        cache[4] = 104; cache[5] = 240;  cache[6] = 544;
        cache_len = 6;
    }

    /* enlarge the cache if necessary */
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) { free(cache); return 0; }
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute any terms that have not yet been computed */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        cache[k] = (cache[k - 2] * 6ULL) - (cache[k - 4] * 4ULL);
    }
    cache_len = n;

    return cache[n];
}
