/*
 * A292878  Number of ascending ballistic random walks of length n in
 *          3-dimensions.
 * https://oeis.org/A292878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2*a(n-3), a(0)=1, a(1)=5, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure cache is large enough to hold at least n */
    if (n >= (idx_t)cache_size) {
        size_t need = (size_t)n + 1;
        if (need < 3) need = 3;          /* keep room for initial values */

        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – abort gracefully */
            abort();
        }
        cache = tmp;

        /* initialize base terms if they are not yet set */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)5;
            cache[2] = (seq_t)21;
        }

        /* compute new entries using the recurrence */
        for (size_t i = cache_size; i < need; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)5;
            } else if (i == 2) {
                cache[i] = (seq_t)21;
            } else {
                seq_t t = mul_small(cache[i-1], 4);
                t = t - cache[i-2];
                t = t + mul_small(cache[i-3], 2);
                cache[i] = t;
            }
        }
        cache_size = need;
    }
    return cache[(size_t)n];
}
