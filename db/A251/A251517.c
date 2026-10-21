/*
 * A251517  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 2 3 4 5 6 7 8 9 or 10.
 * https://oeis.org/A251517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) + 8*a(n-2) - 20*a(n-3) for n ≥ 4 Initial
 *            values: a(1) = 246, a(2) = 3798, a(3) = 58618 This recurrence
 *            reproduces all known terms of A251517 and allows efficient
 *            computation for any n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a(n) for sequence A251517. */
seq_t a(idx_t n)
{
    /* Cache of already computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* Allocated length (index 0 unused) */

    if (n == 0)                 /* undefined offset; return 0 */
        return (seq_t)0;

    /* Ensure the cache can store up to index n. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();            /* Memory allocation failed */
        /* Initialise new slots to zero (sentinel value). */
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values sequentially up to n, re‑using cached results. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            if (cache[1] == (seq_t)0)
                cache[1] = (seq_t)246;
        } else if (i == 2) {
            if (cache[2] == (seq_t)0)
                cache[2] = (seq_t)3798;
        } else if (i == 3) {
            if (cache[3] == (seq_t)0)
                cache[3] = (seq_t)58618;
        } else {
            /* Apply recurrence: a(i) = 15*a(i-1) + 8*a(i-2) - 20*a(i-3) */
            cache[i] = ((seq_t)15 * cache[i-1]) +
                       ((seq_t)8  * cache[i-2]) -
                       ((seq_t)20 * cache[i-3]);
        }
    }

    return cache[n];
}
