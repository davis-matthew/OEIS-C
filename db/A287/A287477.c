/*
 * A287477  a(0) = 0; a(1) = 1; a(2*n) = n - a(a(n)), a(2*n+1) = a(n-a(n)) +
 *          a(n-a(n+1)).
 * https://oeis.org/A287477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(1) = 1 * a(2*n) = n - a(a(n)) * a(2*n+1) =
 *            a(n-a(n)) + a(n-a(n+1))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* capacity of the cache array */
    static idx_t next  = 0;   /* next index that is not yet computed */

    /* Handle the two trivial start values */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        next   = 2;          /* the first index that is still unknown */
    }

    /* Grow the cache array until it can hold index n */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero initialise the newly allocated space */
        if (new_cap > cap) {
            memset((char*)tmp + cap * sizeof(seq_t),
                   0, (new_cap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n (inclusive) that are still missing.
     * The recurrence references only earlier indices, so we can
     * proceed in increasing order. */
    for (idx_t i = next; i <= n; ++i) {
        if ((i & 1u) == 0) {          /* even: i = 2*k                */
            idx_t k = i >> 1;         /* k = i/2                      */
            seq_t ak  = cache[k];
            seq_t aak = cache[(idx_t)ak];
            cache[i] = (seq_t)k - aak;
        } else {                      /* odd:  i = 2*k+1              */
            idx_t k = i >> 1;                 /* k = (i-1)/2               */
            idx_t idx1 = k - (idx_t)cache[k];
            idx_t idx2 = k - (idx_t)cache[k + 1];
            seq_t v1 = cache[idx1];
            seq_t v2 = cache[idx2];
            cache[i] = v1 + v2;
        }
    }

    next = n + 1;      /* all indices up to n are computed */
    return cache[n];
}
