/*
 * A384641  Expansion of (1+2*x-x^3) / (1-x-5*x^2+x^3+2*x^4).
 * https://oeis.org/A384641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 929 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 5*a(n-2) - a(n-3) - 2*a(n-4) for n >= 4
   with initial values a(0)=1, a(1)=3, a(2)=8, a(3)=21
*/
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Initial values for the first four terms */
    static const seq_t init[4] = {1, 3, 8, 21};

    /* Cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Direct return for indices < 4 */
    if (n < 4)
        return init[(size_t)n];

    /* First-time initialization of the cache */
    if (cache == NULL) {
        /* Allocate space for at least the first four terms */
        cache_len = 4;
        cache = mem_new(seq_t, cache_len);
        if (cache == NULL)
            return 0;       /* allocation failure – return zero */
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
    }

    /* If the requested term is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Ensure the buffer is large enough to hold n+1 terms */
    idx_t needed_len = n + 1;
    if (needed_len > cache_len) {
        seq_t *tmp = mem_resize(seq_t, cache, needed_len);
        if (tmp == NULL)
            return 0;       /* allocation failure – return zero */
        cache = tmp;

        /* Compute missing terms from the current cache length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Use the recurrence relation */
            cache[i] = cache[i - 1]
                     + 5 * cache[i - 2]
                     - cache[i - 3]
                     - 2 * cache[i - 4];
        }
        cache_len = needed_len;
    }

    return cache[n];
}
