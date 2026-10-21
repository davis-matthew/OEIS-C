/*
 * A289666  a(n) = number of weakly threshold graphs on n nodes.
 * https://oeis.org/A289666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A289666: number of weakly threshold graphs on n nodes.
    Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3) - a(n-5)  for n>5
    Initial values:
        a(0)=0
        a(1)=1
        a(2)=2
        a(3)=4
        a(4)=9
        a(5)=21
    The function is memoised so that any order of calls is supported.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;     /* array of a(0)..a(max_cached_n) */
    static idx_t cached_len = 0;    /* number of elements stored (i.e. max_cached_n+1) */

    /* Return immediately if the value is already computed. */
    if (n < cached_len) {
        return cache[n];
    }

    /* Need to extend the cache up to n. */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort. */
        abort();
    }
    cache = new_cache;

    /* Compute missing terms.  Loop starts at the previous length. */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else if (i == 3) {
            cache[i] = 4;
        } else if (i == 4) {
            cache[i] = 9;
        } else if (i == 5) {
            cache[i] = 21;
        } else {
            /* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3) - a(n-5) */
            seq_t t = 4 * cache[i - 1];        /* 4*a(n-1)   */
            t -= 3 * cache[i - 2];            /* -3*a(n-2)  */
            t -= cache[i - 3];                /* -a(n-3)    */
            t -= cache[i - 5];                /* -a(n-5)    */
            cache[i] = t;
        }
    }

    /* Update cached length. */
    cached_len = new_len;
    return cache[n];
}
