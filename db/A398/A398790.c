/*
 * A398790  Number of ways to tile a 1 X n strip using tiles of length 1 or 2,
 *          each tile colored red or blue, such that no two red tiles are
 *          adjacent.
 * https://oeis.org/A398790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) + a(n-4) for n ≥ 4 with
 *            a(0)=1, a(1)=2, a(2)=5, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cached sequence */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* initialise cache first time */
    if (cache == NULL) {
        size_t sz = (n >= 4 ? (size_t)n + 1 : 4);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)11;
        max_computed = 3;
    }

    /* extend cache if needed */
    if (n > max_computed) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     + (seq_t)2 * cache[i-3]
                     + cache[i-4];
        }
        max_computed = n;
    }

    return cache[n];
}
