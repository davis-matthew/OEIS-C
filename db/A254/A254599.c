/*
 * A254599  Numbers of words on alphabet {0,1,...,9} with no subwords ii, for
 *          i from {0,1}.
 * https://oeis.org/A254599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 8*a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS A254599. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;          /* allocated slots in cache */
    static idx_t last    = (idx_t)-1;    /* highest index calculated */

    /*--- Initialise the cache if this is the first call. ------------*/
    if (cache == NULL) {
        cap = (size_t)(n < 2 ? 2 : n + 1);        /* at least 0..1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* graceful failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        last    = 1;
    }
    /*--- Grow the cache if required. --------------------------------*/
    else if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* graceful failure */
        cache = tmp;
        cap   = new_cap;
    }
    /*--- Compute any missing values using the recurrence. -----------*/
    if ((idx_t)n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[(size_t)i] =
                (seq_t)9 * cache[(size_t)(i - 1)]
              + (seq_t)8 * cache[(size_t)(i - 2)];
        }
        last = n;
    }
    return cache[(size_t)n];
}
