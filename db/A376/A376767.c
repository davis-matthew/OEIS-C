/*
 * A376767  a(1) = 1; thereafter, a(n) = c*a(n-1)^2 + a(n-1), where c=4 if n
 *          is even, c=2 if n is odd.
 * https://oeis.org/A376767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = c * a(n-1)^2 + a(n-1), where c = 4 if n is
 *            even, c = 2 if n is odd
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values. */
    static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
    static idx_t cache_len = 0;     /* length of cache */

    if (n == 0) { /* Undefined for n == 0, but return 0 to avoid UB */
        return 0;
    }

    /* Resize cache if necessary. */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;           /* a(1) */
            } else {
                seq_t prev = cache[i - 1];        /* a(i)  */
                unsigned int c = (((i + 1) & 1) == 0) ? 4 : 2; /* (i+1) is n */
                cache[i] = (seq_t)c * prev * prev + prev;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
