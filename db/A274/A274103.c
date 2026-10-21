/*
 * A274103  Erroneous version of A003323.
 * https://oeis.org/A274103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*a(n-1) - (n-2), with a(1) = 3, defined for n ≥ 1
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 5, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* size of cache array (including index 0) */
    static idx_t last = 0;        /* greatest index that has been computed */

    if (n == 0)
        return 0;   /* undefined; return 0 for safety */

    /* Grow cache if necessary */
    if (cache_cap < n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – abort */
            abort();
        }
        cache = new_cache;
        cache_cap = n + 1;
    }

    /* Compute all terms up to n that have not yet been computed */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)3;
        } else {
            /* a(i) = i * a(i-1) - (i-2) */
            seq_t prod = (seq_t)i * cache[i - 1];
            cache[i] = prod - ((seq_t)i - 2);
        }
    }

    last = n;
    return cache[n];
}
