/*
 * A307272  Partial sums of A307202.
 * https://oeis.org/A307272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9); Initial
 *            values: a(0)=1, a(1)=4, a(2)=13, a(3)=28, a(4)=49, a(5)=73,
 *            a(6)=103, a(7)=145, a(8)=187
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;   /* allocated size of cache */
    static idx_t max_computed = 8;     /* highest index with a defined value */
    static const seq_t init_vals[9] = {
        1, 4, 13, 28, 49, 73, 103, 145, 187
    };

    /* Allocate cache and store the first 9 terms on first call */
    if (cache == NULL) {
        cache_capacity = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) return (seq_t)0;          /* allocation failed */
        for (idx_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
    }

    /* If requested n is beyond the cache, grow it */
    if (n + (idx_t)1 > cache_capacity) {
        idx_t new_cap = cache_capacity * 2;
        if (new_cap < n + (idx_t)1) new_cap = n + (idx_t)1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* If more terms are required, compute them using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + (idx_t)1; i <= n; ++i) {
            cache[i] = cache[i-1]
                      + 2 * cache[i-4]
                      - 2 * cache[i-5]
                      -     cache[i-8]
                      +     cache[i-9];
        }
        max_computed = n;
    }

    return cache[n];
}
