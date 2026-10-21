/*
 * A254857  Octagonal numbers (A000567) that are also centered heptagonal
 *          numbers (A069099).
 * https://oeis.org/A254857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+674*a(n-2)-674*a(n-3)-a(n-4)+a(n-5) with initial
 *            values: a(1)=1, a(2)=8, a(3)=736, a(4)=5461, a(5)=496133
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n<1, but return 0 as a placeholder */
    if (n < 1ULL)
        return (seq_t)0;

    /* static cache: cache[1] .. cache[cache_max] are valid */
    static seq_t *cache    = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);            /* allocate up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms sequentially */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)       cache[i] = (seq_t)1;
        else if (i == 2)  cache[i] = (seq_t)8;
        else if (i == 3)  cache[i] = (seq_t)736;
        else if (i == 4)  cache[i] = (seq_t)5461;
        else if (i == 5)  cache[i] = (seq_t)496133;
        else {
            /* recurrence: a[i] = a[i-1] + 674*a[i-2] - 674*a[i-3] - a[i-4] + a[i-5] */
            seq_t term1 = cache[i - 1]
                        + (seq_t)674 * cache[i - 2];
            seq_t term2 = (seq_t)674 * cache[i - 3] + cache[i - 4];
            cache[i] = term1 - term2 + cache[i - 5];
        }
    }
    cache_max = n;

    return cache[n];
}
