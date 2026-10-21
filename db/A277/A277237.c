/*
 * A277237  Number of strings of length n composed of symbols from the
 *          circular list [1,2,3,4,5,6] such that adjacent symbols in the
 *          string must be adjacent in the list. No runs of length 2 or more
 *          are allowed for symbols 1, 3 and 5.
 * https://oeis.org/A277237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) (for n ≥ 3); a(0)=1, a(1)=6, a(2)=15
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: values already computed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* number of elements stored (cache_len = highest index + 1) */

    /* initialise cache on first call */
    if (!cache) {
        cache_len = 3;                   /* we need indices 0,1,2 */
        cache = (seq_t*)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed, return 0 as safe fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)15;
    }

    /* if n is within the current cache, return it directly */
    if (n + 1 <= cache_len)
        return cache[n];

    /* grow the cache to hold the requested index (realloc) */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;     /* allocation failed */

    /* compute new values up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            new_cache[i] = (seq_t)1;
        } else if (i == 1) {
            new_cache[i] = (seq_t)6;
        } else if (i == 2) {
            new_cache[i] = (seq_t)15;
        } else {
            new_cache[i] = new_cache[i-1] + (seq_t)4 * new_cache[i-2];
        }
    }

    /* update cache pointers and length */
    cache = new_cache;
    cache_len = new_len;

    return cache[n];
}
