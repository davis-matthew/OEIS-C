/*
 * A292034  Linear divisibility sequence based on Salem number of order 4
 *          (case t=6, see formula).
 * https://oeis.org/A292034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) + 6*a(n-3) - a(n-4) with initial
 *            values a(1)=1, a(2)=6, a(3)=29, a(4)=144 (valid for n > 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* cache[0] unused, cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* highest n computed so far */

    if (n < 1) return 0;           /* undefined for n < 1 */

    /* first call: allocate space for 4 terms and initialise them */
    if (!cache) {
        cache_cap = 4;
        cache = alloc_seq(cache_cap + 1);   /* alloc_seq provides seq_t* */
        if (!cache) return 0;               /* allocation failed */
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 29;
        cache[4] = 144;
    }

    /* compute missing terms on demand */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* realloc failed */
        for (idx_t i = cache_cap + 1; (size_t)i <= n; ++i) {
            cache[i] =
                6 * cache[i - 1]
              - 6 * cache[i - 2]
              + 6 * cache[i - 3]
              - cache[i - 4];
        }
        cache_cap = n;
    }

    return cache[n];
}
