/*
 * A298678  Start with the hexagonal tile of the Shield tiling and recursively
 *          apply the substitution rule. a(n) is the number of hexagonal tiles
 *          after n iterations.
 * https://oeis.org/A298678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3), a(0)=1, a(1)=0, a(2)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence   */
static size_t cache_size = 0;        /* allocated array size            */
static idx_t  cache_max  = 2;        /* largest index with computed value */

/* Return the n-th term of A298678.  The function is memoised and can be
   called in arbitrary order.  The recurrence a(n) = 2*a(n-1) + 7*a(n-2)
   - 2*a(n-3) holds for all n > 2.  */
seq_t a(idx_t n) {
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;

    /* Allocate or grow the cache as necessary */
    if (cache == NULL) {
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)7;
        cache_max = 2;
    } else if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;     /* reallocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        cache[i] = t1 * (seq_t)2 + t2 * (seq_t)7 - t3 * (seq_t)2;
    }
    cache_max = n;
    return cache[n];
}
