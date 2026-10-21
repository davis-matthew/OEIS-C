/*
 * A281208  Number of 4 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(2) = 38 a(3) = 168 a(4) = 270 a(5) = 470 a(6) = 804
 *            a(7) = 1358 a(8) = 2284 a(9) = 3834 a(10) = 6432 a(n) = 4*a(n-1)
 *            - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) for n>10
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 36, u64 n <= 81, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Indexes start at 1 */
    if (n < 1)
        return 0;  /* undefined for n<1, return 0 */

    /* Cache to memoise results */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with first 10 terms on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..10 (index 0 unused) */
        cache = (seq_t *)malloc((10 + 1) * sizeof(seq_t));
        if (!cache)
            return 0;  /* allocation failure */

        cache[0] = 0;          /* unused */
        cache[1] = 0;
        cache[2] = 38;
        cache[3] = 168;
        cache[4] = 270;
        cache[5] = 470;
        cache[6] = 804;
        cache[7] = 1358;
        cache[8] = 2284;
        cache[9] = 3834;
        cache[10] = 6432;

        cache_size = 10;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* Grow cache to hold up to index n */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure */
        cache = tmp;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* For i <= 10 we already have values */
        if (i <= 10)
            continue;

        seq_t val = 4 * cache[i - 1]
                    - 4 * cache[i - 2]
                    - 2 * cache[i - 3]
                    + 4 * cache[i - 4]
                    - cache[i - 6];
        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
