/*
 * A275402  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=16, a(3)=84, a(n)=4*a(n-1)+6*a(n-2)+4*a(n-3) */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Memoized cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current allocated size */

    if (n < 1)
        return (seq_t)0;   /* below offset – undefined; return 0 */

    size_t needed = (size_t)n;
    size_t old_cap = cache_cap;   /* terms already available */

    /* Grow cache if necessary */
    if (needed > cache_cap) {
        seq_t *new_cache = alloc_seq_array(needed);
        if (cache != NULL) {
            for (size_t i = 0; i < old_cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = needed;
    }

    /* Compute any missing terms up to n */
    for (size_t i = old_cap; i < needed; ++i) {
        if (i == 0)
            cache[i] = (seq_t)3;
        else if (i == 1)
            cache[i] = (seq_t)16;
        else if (i == 2)
            cache[i] = (seq_t)84;
        else
            cache[i] = cache[i - 1] * ((seq_t)4) +
                       cache[i - 2] * ((seq_t)6) +
                       cache[i - 3] * ((seq_t)4);
    }

    return cache[n - 1];
}
