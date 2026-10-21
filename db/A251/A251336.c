/*
 * A251336  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to a nonzero multiple of 3.
 * https://oeis.org/A251336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2) - 25*a(n-3) initial values:
 *            a(1)=85, a(2)=455, a(3)=2450 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Treat requests with n==0 as undefined, return 0 */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  last_computed = 0;

    /* Resize cache if necessary (+1 for index n) */
    if (cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – in practice we could abort or return 0 */
            if (cache) free(cache);
            cache = NULL;
            cache_cap = 0;
            return (seq_t)0;
        }
        cache   = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* Compute values up to n, using memoisation */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)85;
        } else if (i == 2) {
            cache[2] = (seq_t)455;
        } else if (i == 3) {
            cache[3] = (seq_t)2450;
        } else {
            /* recurrence: a(n) = 10*a(n-1) – 20*a(n-2) – 25*a(n-3) */
            cache[i] = (seq_t)10 * cache[i - 1]
                     - (seq_t)20 * cache[i - 2]
                     - (seq_t)25 * cache[i - 3];
        }
    }
    last_computed = n;
    return cache[n];
}
