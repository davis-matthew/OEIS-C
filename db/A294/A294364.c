/*
 * A294364  Linear recurrence with signature (1,1,-1,1,1), where the first
 *          terms are powers of 2 (1,2,4,8,16).
 * https://oeis.org/A294364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)+a(n-5); a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8, a(4)=16
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial five terms */
    static const seq_t init[5] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16};

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static idx_t cache_used = 0;   /* number of entries already stored (max index + 1) */

    /* Ensure capacity */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if needed */
    for (idx_t i = cache_used; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (i < 5) {
            cache[idx] = init[i];
        } else {
            cache[idx] = cache[idx-1] + cache[idx-2]
                        - cache[idx-3] + cache[idx-4] + cache[idx-5];
        }
    }
    cache_used = n + 1;

    return cache[(size_t)n];
}
