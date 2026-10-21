/*
 * A247325  Number of paths from (0,0) to (n,2), with vertices (i,k)
 *          satisfying 0 <= k <= 3, consisting of segments given by the
 *          vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4) * Initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 1, a(3) = 4 * This implementation uses
 *            memoization and runs in O(n) time
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoized values of the sequence   */
static size_t cache_len = 0;    /* smallest index not yet computed  */
static size_t cache_cap = 0;    /* allocated capacity of cache      */

seq_t a(idx_t n)
{
    /* handle the initial four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* allocate cache once, with at least space for the first 4 elements */
    if (cache == NULL) {
        cache_cap = 4 > (size_t)n + 1 ? 4 : (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);   /* out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache_len = 4;
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);     /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)3 * cache[i - 2]) + ((seq_t)2 * cache[i - 3]) - cache[i - 4];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
