/*
 * A347205  a(2n+1) = a(n) for n >= 0, a(2n) = a(n) + a(n - 2^A007814(n)) for
 *          n > 0 with a(0) = 1.
 * https://oeis.org/A347205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(2n+1) = a(n), a(2n) = a(n) + a(n - 2^A007814(n)), * a(0) = 1
 *            (A007814(n) = exponent of the largest power of two dividing n)
 * Range:     u8 n <= 1255, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Expand cache if necessary and fill new entries */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure protection */
        cache = tmp;

        /* Initialise base case if not yet set */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache_size = 1;
        }

        /* Compute values from old size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) continue;   /* already handled */

            if (i & 1) {                  /* odd index: a(2m+1) = a(m) */
                cache[i] = cache[i >> 1];
            } else {                       /* even index */
                idx_t half = i >> 1;       /* m */
                unsigned int tz = 0;
                idx_t temp = half;
                while ((temp & 1ULL) == 0) {   /* count trailing zeros of m */
                    ++tz;
                    temp >>= 1;
                }
                idx_t shift = ((idx_t)1 << tz);  /* 2^tz */
                cache[i] = cache[half] + cache[half - shift];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
