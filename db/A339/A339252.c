/*
 * A339252  a(0) = 1, a(1) = 4, a(2) = 11, and a(n) = 4*a(n-1) - 4*a(n-2) for
 *          n >= 3.
 * https://oeis.org/A339252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 11, a(n) = 4*a(n‑1) – 4*a(n‑2) for n
 *            ≥ 3
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] = a(i)            */
    static idx_t cap   = 0;       /* allocated size (number of
                                   indices)                  */
    static idx_t filled = 0;      /* number of indices already
                                   initialized (≤ cap)        */

    /* Allocate the initial cache and set the first three values. */
    if (cap == 0) {
        cap = 4;                       /* just larger than 3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failed   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)11;
        filled = 3;
    }

    /* If the requested index is already cached, return it. */
    if (n < filled)
        return cache[n];

    /* Grow the cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (n >= new_cap)
            new_cap <<= 1;              /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* reallocation failed  */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    idx_t start = (filled > 3) ? filled : 3;  /* start from 3 if not yet
                                               initialized                  */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] - (seq_t)4 * cache[i-2];
    }
    filled = n + 1;

    return cache[n];
}
