/*
 * A267691  a(n) = (n + 1)*(6*n^4 - 21*n^3 + 31*n^2 - 31*n + 30)/30.
 * https://oeis.org/A267691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n+1) = a(n) + n^4 (for n >= 0)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache holding values computed so far.
       Cache length is the number of elements in the array. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Expand cache if required.  The new size is n+1 to store a[0..n]. */
    if (n >= cache_len) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp)
            abort();           /* Allocation failure – abort the program */
        cache = tmp;

        /* Compute missing values from cache_len up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                /* Base value */
                cache[0] = (seq_t)1;
            } else {
                /* Add (i-1)^4 to the previous term. */
                seq_t k   = (seq_t)(i - 1);
                seq_t k2  = k * k;
                seq_t k4  = k2 * k2;
                cache[i] = cache[i - 1] + k4;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
