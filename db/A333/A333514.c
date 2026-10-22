/*
 * A333514  Number of self-avoiding closed paths on an n X 4 grid which pass
 *          through four corners ((0,0), (0,3), (n-1,3), (n-1,0)).
 * https://oeis.org/A333514
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) + 7*a(n-3) - 3*a(n-4) - 2*a(n-5) for
 *            n>6 initial values: a(2)=1, a(3)=3, a(4)=11, a(5)=49, a(6)=229,
 *            a(7)=1081
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _base_vals[6] = {1, 3, 11, 49, 229, 1081};

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* static memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* lazy initialization */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                  /* allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        for (idx_t i = 2; i <= 7; ++i)
            cache[i] = _base_vals[i - 2];
        cache_max = 7;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 7) {                       /* already initialized */
            val = _base_vals[i - 2];
        } else {
            val = (seq_t)7  * cache[i - 1]
                - (seq_t)12 * cache[i - 2]
                + (seq_t)7  * cache[i - 3]
                - (seq_t)3  * cache[i - 4]
                - (seq_t)2  * cache[i - 5];
        }
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
