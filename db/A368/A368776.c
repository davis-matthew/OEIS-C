/*
 * A368776  a(n) = (n+1)^2 * (n!)^3 * Sum_{k=0..n} 1/((k+1)^2 * (k!)^3).
 * https://oeis.org/A368776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368776:
   a(0) = 1,
   a(n) = (n+1)^2 * n * a(n-1) + 1   for n >= 1 */
#include <stdlib.h>

/* Return a(n) for any n >= 0.  The result is memoised so that
   any order of calls is efficient.  seq_t may be an unsigned
   integer type of up to 128 bits or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;    /* allocated capacity (in terms) */
    static size_t cache_len = 0;    /* number of terms stored (next free index) */

    /* Grow the cache array so that it can hold terms 0 … n */
    if ((size_t)n + 1 > cache_cap) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) {
            if (new_cap > SIZE_MAX / 2) {
                new_cap = needed;
                break;
            }
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();   /* allocation failure */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* Initialise a(0) if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* a(i) = (i+1)^2 * i * a(i-1) + 1 */
        seq_t a_prev    = cache[i - 1];
        seq_t factor    = (seq_t)(i + 1) * (seq_t)(i + 1);   /* (i+1)^2        */
        seq_t mult      = (seq_t)i * a_prev;                /* i * a(i-1)     */
        cache[i] = factor * mult + (seq_t)1;
    }

    /* Update the recorded number of cached terms */
    cache_len = (size_t)n + 1;

    return cache[n];
}
