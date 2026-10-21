/*
 * A298163  Number of n X 4 0..1 arrays with every element equal to 3, 4, 5 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-a(n-3)-2*a(n-4)-2*a(n-5)+a(n-6)+a(n-7)
 *            with initial values a(1)=0, a(2)=2, a(3)=2, a(4)=4, a(5)=6,
 *            a(6)=11, a(7)=18. Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 80, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of elements cached */

    /* Ensure cache has enough space for up to n elements */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len < (size_t)n) {
        /* initial values for a(1)..a(7) */
        static const seq_t init[7] = { (seq_t)0, (seq_t)2, (seq_t)2,
                                      (seq_t)4, (seq_t)6, (seq_t)11,
                                      (seq_t)18 };

        for (idx_t i = cache_len; i < n; ++i) {
            idx_t idx = i + 1;   /* 1‑based index of the term to compute */

            if (idx <= 7) {
                cache[i] = init[idx - 1];
            } else {
                /* a(idx) = 2*a(idx-1)+a(idx-2)-a(idx-3)-2*a(idx-4)-2*a(idx-5)+a(idx-6)+a(idx-7) */
                seq_t val =  2 * cache[idx - 2] + cache[idx - 3];
                                val -= cache[idx - 4];
                                val -= 2 * cache[idx - 5];
                                val -= 2 * cache[idx - 6];
                                val += cache[idx - 7];
                                val += cache[idx - 8];
                cache[i] = val;
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
