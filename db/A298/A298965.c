/*
 * A298965  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) +3*a(n-2) -11*a(n-3) -11*a(n-4) +2*a(n-5)
 *            +19*a(n-6) -8*a(n-7) -12*a(n-8) (n ≥ 9) Initial values: a(1)=4,
 *            a(2)=32, a(3)=219, a(4)=1575, a(5)=11283, a(6)=80972,
 *            a(7)=581057, a(8)=4169867
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values, indexed 0 .. cache_len-1
       corresponding to a(1) .. a(cache_len)   */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of values stored (max index computed) */
    static size_t cache_cap = 0;   /* capacity of the cache array */

    /* initial values for n = 1 .. 8 */
    const seq_t init[8] = {4, 32, 219, 1575, 11283, 80972, 581057, 4169867};

    /* Ensure the cache holds at least the first 8 values */
    if (cache_len < 8) {
        size_t need = 8;
        if (cache == NULL) {
            cache = (seq_t *)malloc(need * sizeof(seq_t));
            cache_cap = need;
        } else if (need > cache_cap) {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
            cache_cap = need;
        }
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* Return immediately if requested index is already known */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cache_cap = n;
    }

    /* Compute a(n) using the recurrence, extending the cache */
    for (idx_t i = cache_len + 1; i <= n; ++i) {  /* i is 1‑based index */
        seq_t val =  7 * cache[i - 2]
                   + 3 * cache[i - 3]
                   - 11 * cache[i - 4]
                   - 11 * cache[i - 5]
                   +  2 * cache[i - 6]
                   + 19 * cache[i - 7]
                   -  8 * cache[i - 8]
                   - 12 * cache[i - 9];
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
