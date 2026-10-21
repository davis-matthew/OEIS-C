/*
 * A251344  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 3 4 or 5.
 * https://oeis.org/A251344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 12*a(n-3) Initial values: a(1)=51,
 *            a(2)=309, a(3)=1899
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;                       /* largest n filled in cache */

    if (n == 0)
        return 0;                                   /* out of bounds, return 0 */

    /* Allocate the cache if this is the first call */
    if (cache == NULL) {
        size_t sz = n + 1;
        if (sz < 4) sz = 4;                         /* ensure room for first three terms */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL)
            return 0;                               /* memory allocation failed */

        /* Fill in the initial values */
        cache[1] = (seq_t)51;
        cache[2] = (seq_t)309;
        cache[3] = (seq_t)1899;
        computed = 3;
    }

    /* Grow the cache and compute missing terms if necessary */
    if (n > computed) {
        size_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                               /* memory allocation failed */
        cache = tmp;

        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1)           cache[i] = (seq_t)51;
            else if (i == 2)     cache[i] = (seq_t)309;
            else if (i == 3)     cache[i] = (seq_t)1899;
            else
                cache[i] = 6 * cache[i - 1] + 3 * cache[i - 2] - 12 * cache[i - 3];
        }
        computed = n;
    }

    return cache[n];
}
