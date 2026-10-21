/*
 * A253675  Centered triangular numbers (A005448) which are also centered
 *          octagonal numbers (A016754).
 * https://oeis.org/A253675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+9602*a(n-2)-9602*a(n-3)-a(n-4)+a(n-5); Initial
 *            terms: a(1)=1, a(2)=361, a(3)=6241, a(4)=3463321, a(5)=59923081
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base cases for indices 1..5 */
    static const seq_t init_vals[5] = {
        (seq_t)1,
        (seq_t)361,
        (seq_t)6241,
        (seq_t)3463321,
        (seq_t)59923081
    };

    /* Return base value if n is within the first 5 terms */
    if (n <= 5)
        return init_vals[n - 1];

    /* Cache for already computed values, grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Convert idx_t to size_t; n is guaranteed to be small in practice */
    size_t target = (size_t)n;

    if (cache == NULL) {
        /* Start with capacity at least 10 to reduce reallocs */
        cache_size = target < 10 ? target : 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* Initialize the cache with the first 5 terms */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
    } else if (target > cache_size) {
        /* Grow the cache while it is too small */
        size_t new_size = cache_size;
        while (new_size < target)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* if realloc fails, keep old cache */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all missing terms up to target */
    for (size_t i = 5; i < target; ++i) {
        /* all subtractions are safe because the sequence is strictly increasing */
        seq_t diff1 = cache[i - 1] - cache[i - 4];       /* a[i-1] - a[i-4] */
        seq_t diff2 = cache[i - 2] - cache[i - 3];       /* a[i-2] - a[i-3] */
        /* a[i] = a[i-5] + diff1 + 9602*diff2 */
        cache[i] = cache[i - 5] + diff1 + (seq_t)9602 * diff2;
    }

    return cache[target - 1];
}
