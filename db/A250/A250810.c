/*
 * A250810  Number of (n+1) X (6+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3); Initial values: a(1) =
 *            1296, a(2) = 5139, a(3) = 18537
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 to avoid segmentation fault. */
    if (n == 0) return 0;

    /* Static cache stores computed values; index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* largest n computed so far (1‑indexed) */
    static size_t cache_cap = 0;   /* allocated capacity (array length) */

    /* Ensure the cache can hold indices up to n. */
    size_t needed = (size_t)(n + 1);   /* +1 because we ignore index 0 */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* sanity check on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        size_t idx = (size_t)i;    /* index in the array */
        if (i == 1)
            cache[idx] = 1296;
        else if (i == 2)
            cache[idx] = 5139;
        else if (i == 3)
            cache[idx] = 18537;
        else
            cache[idx] = (seq_t)6 * cache[idx - 1]
                       - (seq_t)11 * cache[idx - 2]
                       + (seq_t)6 * cache[idx - 3];
    }

    cache_len = n;
    return cache[(size_t)n];
}
