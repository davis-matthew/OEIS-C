/*
 * A251146  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5) (n ≥ 6), with
 *            initial values: a(1) = 213, a(2) = 299, a(3) = 433, a(4) = 667,
 *            a(5) = 1061
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* values a(1), a(2), … stored at cache[idx-1] */
    static idx_t size_comp = 0;      /* number of values currently stored */
    static idx_t capacity = 0;       /* allocated capacity of cache */

    if (n == 0)
        return 0;                    /* undefined for n < 1, but safe return */

    /* Initialize cache with first 5 values on first call */
    if (!cache) {
        capacity = 8;                /* small initial capacity */
        cache = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!cache)
            exit(1);                 /* allocation failure */
        cache[0] = (seq_t)213;
        cache[1] = (seq_t)299;
        cache[2] = (seq_t)433;
        cache[3] = (seq_t)667;
        cache[4] = (seq_t)1061;
        size_comp = 5;
    }

    /* If requested n is already computed, return it */
    if (n <= size_comp)
        return cache[n - 1];

    /* Grow the cache to accommodate up to n */
    if (n > capacity) {
        capacity = n;
        seq_t *tmp = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp)
            exit(1);                 /* allocation failure */
        cache = tmp;
    }

    /* Compute new values up to n using the recurrence */
    for (idx_t i = size_comp + 1; i <= n; ++i) {
        /* recurrence only applies for i ≥ 6; for i = 6..n we have all needed previous terms */
        seq_t val = (seq_t)3 * cache[i - 2]
                  - (seq_t)5 * cache[i - 4]
                  + cache[i - 5]
                  + (seq_t)2 * cache[i - 6];
        cache[i - 1] = val;
    }
    size_comp = n;

    return cache[n - 1];
}
