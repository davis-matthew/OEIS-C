/*
 * A297458  Number of 2 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) + 4*a(n-4) (n ≥ 5) Initial
 *            values: a(1)=4, a(2)=8, a(3)=25, a(4)=57 Offset: 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms, 0‑based array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)        /* undefined for offset < 1 */
        return 0;

    /* Grow the cache if it does not contain the requested term */
    if (cache_size < n) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;        /* allocation failed */

        /* Copy any existing terms */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Compute newly required terms */
        for (idx_t i = cache_size; i < new_size; ++i) {   /* i is 0‑based */
            idx_t idx = i + 1;  /* 1‑based index for the recurrence */

            if (idx == 1)
                new_cache[i] = (seq_t)4;
            else if (idx == 2)
                new_cache[i] = (seq_t)8;
            else if (idx == 3)
                new_cache[i] = (seq_t)25;
            else if (idx == 4)
                new_cache[i] = (seq_t)57;
            else
                new_cache[i] = new_cache[i-1]
                               + (seq_t)3 * new_cache[i-2]
                               + new_cache[i-3]
                               + (seq_t)4 * new_cache[i-4];
        }

        /* Release old cache and update state */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n-1];
}
