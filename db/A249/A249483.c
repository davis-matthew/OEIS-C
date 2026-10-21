/*
 * A249483  Squares (A000290) which are also centered triangular numbers
 *          (A005448).
 * https://oeis.org/A249483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+98*a(n-2)-98*a(n-3)-a(n-4)+a(n-5) Initial values
 *            (1‑based indexing): a(1)=1, a(2)=4, a(3)=64, a(4)=361, a(5)=6241
 *            Computation is memoised; works for any idx_t ≥1 and any seq_t
 *            implementation that supports + - * / and casting from constant
 *            integers
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[1]..[cache_len] contain computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;

    if (n < 1)
        return 0;                      /* undefined for n < 1, return 0 */

    /* initialise cache with first five terms if not yet done */
    if (cache_len < 5) {
        size_t init = 6;              /* allocate indices 0..5 */
        cache = (seq_t *)realloc(cache, init * sizeof(seq_t));
        if (!cache)
            return 0;                 /* allocation failed */

        cache[0] = 0;                 /* unused dummy */
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 64;
        cache[4] = 361;
        cache[5] = 6241;
        cache_len = 5;
    }

    /* if the requested term is already computed, return it */
    if (n <= cache_len)
        return cache[n];

    /* grow cache to hold terms up to n (inclusive) */
    size_t new_len = (size_t)n;
    cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!cache)
        return 0;                     /* allocation failed */

    /* compute missing terms sequentially */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = cache[i - 1]                          /* a(i-1)   */
                 + (seq_t)98 * cache[i - 2]              /* +98*a(i-2) */
                 - (seq_t)98 * cache[i - 3]              /* -98*a(i-3) */
                 - cache[i - 4]                          /* -a(i-4)   */
                 + cache[i - 5];                        /* +a(i-5)   */
    }
    cache_len = new_len;

    return cache[n];
}
