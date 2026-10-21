/*
 * A271388  a(n) = 4*a(n-1) + a(n-2) - n for n > 1, with a(0) = 0, a(1) = 1.
 * https://oeis.org/A271388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - n (n > 1) Initial values: a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static idx_t   max_computed = 0;

    /* first call: allocate and initialise the cache for n = 0, 1 */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
        if (n < 2) return cache[n];
    }

    /* enlarge cache if needed, doubling its size each time */
    if (n + 1 > cache_len) {
        size_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* if the requested value is already known, return it */
    if (n <= max_computed) return cache[n];

    /* compute all missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t = (seq_t)4 * cache[i - 1] + cache[i - 2] - (seq_t)i;
        cache[i] = t;
    }
    max_computed = n;
    return cache[n];
}
