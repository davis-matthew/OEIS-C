/*
 * A261373  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00001011 00010101 or 01010101.
 * https://oeis.org/A261373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 42, u64 n <= 90, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A261373: Number of (n+2)×3 binary arrays whose each 3×3 subblock has one of the
   clockwise perimeter patterns 00001011, 00010101 or 01010101.        */
/* Empirical recurrence:  a(n) = a(n-2) + 3*a(n-4) + 2*a(n-6)   for n>12.  */
/* Initial values (n=1..12): 36, 39, 60, 97, 154, 247, 392, 618, 977, 1548, 2461, 3894. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* largest index stored + 1 */

    /* hard‑coded initial values */
    static const seq_t init[13] = {
        0ULL,                     /* dummy 0 index */
        36ULL, 39ULL, 60ULL, 97ULL, 154ULL, 247ULL,
        392ULL, 618ULL, 977ULL, 1548ULL, 2461ULL, 3894ULL
    };

    /* return a precomputed small term */
    if (n <= 12ULL) return init[n];

    /* extend cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1ULL;                 /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();                         /* out of memory */
        cache = tmp;

        /* fill in the first 12 values if not already done */
        if (cache_size <= 12ULL) {
            for (idx_t i = cache_size; i <= 12ULL; ++i)
                cache[i] = init[i];
        }

        /* compute remaining terms using the recurrence */
        idx_t start = (cache_size <= 12ULL) ? 13ULL : cache_size;
        for (idx_t i = start; i <= n; ++i) {
            seq_t v = cache[i - 2ULL];
            v += (seq_t)3ULL * cache[i - 4ULL];
            v += (seq_t)2ULL * cache[i - 6ULL];
            cache[i] = v;
        }
        cache_size = n + 1ULL;   /* we've computed up to n */
    }

    return cache[n];
}
