/*
 * A297665  Number of chordless cycles in the n-web graph.
 * https://oeis.org/A297665
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-5) + a(n-6) for n
 *            >= 10 Initial values: a(3)=3, a(4)=10, a(5)=17, a(6)=26,
 *            a(7)=37, a(8)=54, a(9)=83 The array cache stores computed
 *            values; uncomputed entries are zero. The implementation is fully
 *            generic; it uses only integer operations and standard...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed a(n) values      */
static idx_t cache_len = 0;      /* number of cache entries currently allocated */

/* Ensure the cache can store values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)                /* already large enough          */
        return;

    idx_t new_len = n + 1;
    /* realloc may return NULL; we cast for C++ compatibility       */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (new_cache == NULL) {
        abort();                      /* allocation failed             */
    }

    /* initialise new slots to zero (they will be filled later)    */
    for (idx_t i = cache_len; i < new_len; ++i) {
        new_cache[(size_t)i] = 0;
    }

    cache = new_cache;
    cache_len = new_len;
}

/* Sequence values for the base indices 3 … 9 (inclusive).  These are
   the first terms of the sequence; the recurrence below is valid only
   from n = 10 onward. */
static const seq_t base[10] = {
    0, 0, 0, 3, 10, 17, 26, 37, 54, 83
};

seq_t a(idx_t n)
{
    if (n < 3)                    /* undefined for n < 3, return 0 */
        return 0;

    /* Keep track of the previous cache length – this will be the start
       index for any new values that need to be computed. */
    idx_t prev_len = cache_len;

    ensure_cache(n);

    /* If already computed, simply return the cached value. */
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    /* Compute any missing entries from prev_len up to n (inclusive). */
    for (idx_t i = prev_len; i <= n; ++i) {
        if (i <= 9) {
            cache[(size_t)i] = base[(size_t)i];
        } else {
            /* Use the recurrence valid for n >= 10:             */
            /* a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-5) + a(n-6) */
            seq_t term = 4 * cache[(size_t)(i - 1)];
            term -= 6 * cache[(size_t)(i - 2)];
            term += 4 * cache[(size_t)(i - 3)];
            term -= 2 * cache[(size_t)(i - 5)];
            term += cache[(size_t)(i - 6)];
            cache[(size_t)i] = term;
        }
    }

    return cache[(size_t)n];
}
