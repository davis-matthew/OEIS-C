/*
 * A248078  a(1) = 1; a(n+1) = a(n) + product of digits of a(n) + sum of
 *          digits of a(n).
 * https://oeis.org/A248078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = a(n) + digit_product(a(n)) + digit_sum(a(n))  (n ≥ 1) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n = 0 */

    /* Static cache: cache[i] holds a(i+1)   (0‑based array) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* capacity of cache array */

    /* Ensure the cache has space for up to n terms (indices 0..n-1) */
    if (n > cache_cap)
    {
        idx_t new_cap = n;          /* exact size is sufficient */
        seq_t *tmp = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failure */

        if (cache)
        {
            memcpy(tmp, cache, (size_t)cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize a(1) if it hasn't been stored yet */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)1;        /* a(1) */
        cache_len = 1;
    }

    /* Compute missing terms up to a(n) */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* previous term a(i-1) is stored at cache[i-2] */
        seq_t prev = cache[i - 2];
        seq_t prod = digit_product(prev);
        seq_t sum  = digit_sum(prev);
        cache[i - 1] = prev + prod + sum;   /* store a(i) */
    }
    cache_len = n;

    return cache[n - 1];
}
