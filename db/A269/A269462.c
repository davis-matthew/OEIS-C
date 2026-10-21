/*
 * A269462  Number of length-n 0..3 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 18*a(n-3), with a(1)=4, a(2)=16, a(3)=60
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Cache of previously computed terms (1‑based). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_n = 0;

    if (n == 0)
        return 0;        /* not defined; harmless guard */

    /* Ensure the cache has room for index n (and index 0 unused). */
    size_t needed = (size_t)n + 1;
    if (needed < 4)          /* we need space for terms 1..3 anyway */
        needed = 4;

    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache)
            return 0;      /* allocation failed – unlikely in tests */
        cache_size = needed;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return 0;      /* allocation failed */
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise base values if not already done. */
    if (max_n < 3) {
        cache[1] = 4;
        cache[2] = 16;
        cache[3] = 60;
        if (max_n < 3)
            max_n = 3;
    }

    /* Grow the sequence up to n using the recurrence. */
    if (n > max_n) {
        size_t start = max_n + 1;          /* start after the last known term */
        for (size_t i = start; i <= (size_t)n; ++i) {
            cache[i] = 5 * cache[i-1] - 18 * cache[i-3];
        }
        max_n = n;
    }

    return cache[n];
}
