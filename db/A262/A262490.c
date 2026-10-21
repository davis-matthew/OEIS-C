/*
 * A262490  The index of the first of two consecutive positive triangular
 *          numbers (A000217) the sum of which is equal to the sum of four
 *          consecutive positive triangular numbers.
 * https://oeis.org/A262490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n>3 with initial values
 *            a(1)=9, a(2)=57, a(3)=337
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static size_t cache_len = 0;  /* number of stored terms */

    /* initialise cache with first three terms once */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure: return 0 */
        cache[0] = (seq_t)9;      /* a(1) */
        cache[1] = (seq_t)57;     /* a(2) */
        cache[2] = (seq_t)337;    /* a(3) */
    }

    /* return already computed value if available */
    if (n <= cache_len) return cache[n-1];

    /* expand cache up to requested index n */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0;  /* allocation failure */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to index a(i+1) */
        seq_t val = (seq_t)7 * cache[i-1] - (seq_t)7 * cache[i-2] + cache[i-3];
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[n-1];
}
