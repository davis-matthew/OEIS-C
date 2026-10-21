/*
 * A276095  A nonlinear recurrence of order 4: a(1)=a(2)=a(3)=a(4)=1;
 *          a(n)=(a(n-1)+a(n-2)+a(n-3))^2/a(n-4).
 * https://oeis.org/A276095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((a(n-1)+a(n-2)+a(n-3))^2) / a(n-4), a(1) = a(2) = a(3) =
 *            a(4) = 1, offset = 1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms; indices 0 .. m-1 correspond to a(1) .. a(m) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of terms stored in cache */

    if (n == 0)   /* undefined, but return 1 (any value suffices) */
        return (seq_t)1;

    /* Ensure the cache contains terms up to a(n) */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if ((size_t)n > new_cap)
            new_cap = (size_t)n;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();              /* allocation failure */
        cache = tmp;

        /* Compute any new terms, filling from cache_cap up to new_cap-1 */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i < 4) {          /* a(1)..a(4) correspond to cache[0..3] */
                cache[i] = (seq_t)1;
                continue;
            }
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
            seq_t sq  = sum * sum;               /* (sum)^2 */
            cache[i] = sq / cache[i-4];          /* exact division */
        }

        cache_cap = new_cap;
    }

    return cache[n-1];  /* cache[0] holds a(1) */
}
