/*
 * A274975  Sum of n-th powers of the three roots of x^3-2*x^2-x+1.
 * https://oeis.org/A274975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=2, a(2)=6; for n>=3, a(n)=2*a(n-1)+a(n-2)-a(n-3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static idx_t capacity = 0;    /* number of cached entries */

    if (n >= capacity) {
        /* Allocate space for all terms up to n */
        size_t new_capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp && new_capacity > 0)
            return 0;           /* allocation failed: return 0 as safe value */

        cache = tmp;
        for (idx_t i = capacity; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)3;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else if (i == 2)
                cache[i] = (seq_t)6;
            else
                cache[i] = (seq_t)2 * cache[i-1] + cache[i-2] - cache[i-3];
        }
        capacity = new_capacity;
    }
    return cache[n];
}
