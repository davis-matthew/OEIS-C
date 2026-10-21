/*
 * A277752  a(n) = Sum_{k=0..n} (-1)^k*floor(phi^k), where phi is the golden
 *          ratio (A001622).
 * https://oeis.org/A277752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-2) - a(n-3) - 2*a(n-4) + a(n-5) Initial values:
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=-2, a(4)=4
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure cache is initialized with the first five values */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-2;
        cache[4] = (seq_t)4;
        cache_len = 5;
    }

    /* If requested n is already available, return it */
    if (n < cache_len) return cache[n];

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-2) - a(i-3) - 2*a(i-4) + a(i-5) */
        seq_t val = (seq_t)3 * cache[i - 2]
                    - cache[i - 3]
                    - (seq_t)2 * cache[i - 4]
                    + cache[i - 5];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
