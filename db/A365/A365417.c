/*
 * A365417  Partial sums of A265036.
 * https://oeis.org/A365417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 12*a(n-2) + 18*a(n-3) - 18*a(n-4) + 12*a(n-5)
 *            - 5*a(n-6) + a(n-7) for n > 10 Initial terms: a(0) = 1 a(1) = 5
 *            a(2) = 11 a(3) = 18 a(4) = 28 a(5) = 42 a(6) = 62 a(7) = 86 a(8)
 *            = 110 a(9) = 133 a(10) = 159
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial terms if cache not yet initialized */
    if (cache_size == 0) {
        /* allocate space for 0..10 (11 terms) */
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 11;
        cache[3] = 18;
        cache[4] = 28;
        cache[5] = 42;
        cache[6] = 62;
        cache[7] = 86;
        cache[8] = 110;
        cache[9] = 133;
        cache[10] = 159;
        cache_size = 11;
    }

    /* return cached value if available */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    /* guard against allocation failure – in that case we simply return 0 */
    if (!new_cache) {
        /* allocation failed; keep old cache */
        return 0;
    }
    cache = new_cache;

    /* compute terms from cache_size to n using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] =
            5 * cache[i - 1]
          - 12 * cache[i - 2]
          + 18 * cache[i - 3]
          - 18 * cache[i - 4]
          + 12 * cache[i - 5]
          - 5 * cache[i - 6]
          + 1 * cache[i - 7];
    }

    cache_size = n + 1;
    return cache[n];
}
