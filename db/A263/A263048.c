/*
 * A263048  a(0)=0, a(1)=1; for n>1, a(n) = a(n-2)^a(n-1) + a(n-1)^a(n-2).
 * https://oeis.org/A263048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-2)^a(n-1) +
 *            a(n-1)^a(n-2) (OEIS A263048)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache to handle arbitrary call order and reuse computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* total capacity of the cache array   */
    static idx_t  computed_max = 1;/* largest index for which cache[i] is valid */

    /* Ensure cache is large enough */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, return 0 as sentinel */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Compute missing entries up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t x = big_pow(cache[i-2], cache[i-1]); /* a(i-2)^a(i-1) */
            seq_t y = big_pow(cache[i-1], cache[i-2]); /* a(i-1)^a(i-2) */
            cache[i] = x + y;
        }
        computed_max = n;
    }

    return cache[n];
}
