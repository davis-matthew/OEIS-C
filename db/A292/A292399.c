/*
 * A292399  p-INVERT of (1,2,3,5,8,...) (distinct Fibonacci numbers), where
 *          p(S) = (1 - S)^2.
 * https://oeis.org/A292399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4) for n >= 5; Base values:
 *            a(0)=2, a(1)=7, a(2)=22, a(3)=69, a(4)=212
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* p-INVERT of the distinct Fibonacci numbers, p(S) = (1 - S)^2   */

seq_t a(idx_t n)
{
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;   /* allocated capacity   */
    static size_t cache_len    = 0;   /* number of values     */
    static bool   initialized  = false;

    /* initialise on first call */
    if (!initialized) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 7;
        cache[2] = 22;
        cache[3] = 69;
        cache[4] = 212;
        cache_len = 5;
        initialized = true;
    }

    /* already computed? */
    if (n < cache_len)
        return cache[n];

    /* grow storage if necessary */
    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 4 * cache[i - 1]             /* 4*a(n-1)   */
                   - 8 * cache[i - 3]             /* 8*a(n-3)   */
                   - 4 * cache[i - 4];            /* 4*a(n-4)   */
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
