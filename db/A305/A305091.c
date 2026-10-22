/*
 * A305091  Number of n X 2 0..1 arrays with every element unequal to 1, 2, 4,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305091
    Formula: a(n) = a(n-1) + a(n-2) + 8*a(n-3) - 16*a(n-5)   for n > 6
    Initial values: a(1)=1, a(2)=3, a(3)=9, a(4)=10, a(5)=27, a(6)=109 */

#include <stdlib.h>

/* external linkage on purpose */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms, 1‑based */
    static idx_t cache_size = 0;     /* maximum index currently cached */

    /* if the requested term is already cached, return it */
    if (n <= cache_size && cache != NULL)
        return cache[n];

    /* need to grow the cache up to n */
    idx_t new_size = n;
    /* allocate new array (size n+1, we ignore index 0) */
    seq_t *new_cache = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
    if (!new_cache)                                   /* allocation failed */
        abort();                                       /* no alternative */

    /* copy old cached values, if any */
    if (cache) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
    }

    /* initial constants for the first six terms */
    const seq_t init[6] = { (seq_t)1, (seq_t)3, (seq_t)9,
                            (seq_t)10, (seq_t)27, (seq_t)109 };

    /* ensure the first six terms are set (where applicable) */
    idx_t upper = (new_size < 6) ? new_size : 6;
    for (idx_t i = 1; i <= upper; ++i)
        new_cache[i] = init[i - 1];

    /* compute further terms using the recurrence */
    /* start at the first index > cache_size that is >= 7 */
    idx_t start = (cache_size + 1 > 7) ? cache_size + 1 : 7;
    if (start < 1)          /* safety for very small n */
        start = 1;
    if (new_size >= 7 && start <= new_size) {
        for (idx_t i = start; i <= new_size; ++i) {
            new_cache[i] =
                new_cache[i - 1] + new_cache[i - 2] +
                (seq_t)8 * new_cache[i - 3] -
                (seq_t)16 * new_cache[i - 5];
        }
    }

    /* free old cache and replace with the new one */
    free(cache);
    cache = new_cache;
    cache_size = new_size;

    return cache[n];
}
