/*
 * A349185  G.f. A(x) satisfies: A(x) = (1 - x) / (1 - 2 * x - x^2 - x^2 *
 *          A(x)).
 * https://oeis.org/A349185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1 a(n) = 2*a(n-1) + a(n-2) + Σ_{k=0}^{n-2}
 *            a(k)*a(n-2-k) (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms currently stored */

    /* grow cache if needed */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        /* allocate (or reallocate) memory for the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory – return 0 as a safe value */
        cache = tmp;

        /* initialise newly allocated slots (they will be overwritten) */
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;

        /* ensure the initial two terms are present */
        if (cache_len == 0) {
            cache[0] = 1;
            if (new_len > 1) cache[1] = 1;
            cache_len = (new_len > 1) ? 2 : 1;
        } else if (cache_len == 1) {
            cache[1] = 1;
            cache_len = 2;
        }

        /* compute missing terms one by one */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i ≥ 2 in this loop */
            seq_t sum = 0;
            for (size_t k = 0; k <= i - 2; ++k)
                sum += cache[k] * cache[i - 2 - k];
            cache[i] = 2 * cache[i - 1] + cache[i - 2] + sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
