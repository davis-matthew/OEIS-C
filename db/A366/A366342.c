/*
 * A366342  a(n) = Product_{k=1..n} Sum_{j=1..k} j^k.
 * https://oeis.org/A366342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} Sum_{j=1..k} j^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Memoized product a(n) = a(n-1) * S_n, with S_k = Sum_{j=1..k} j^k */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0)
        return 0;  /* undefined for n < 1, return 0 */

    /* Extend cache up to required n */
    while (n > cache_size) {
        /* Grow cache by one element */
        size_t new_size = cache_size + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)  /* allocation failure */
            exit(1);
        cache = new_cache;
        cache_size = new_size;

        idx_t k = new_size;      /* current term index (1‑based) */
        seq_t sum = 0;

        /* Compute S_k = sum_{j=1..k} j^k */
        for (idx_t j = 1; j <= k; ++j)
            sum += big_pow((seq_t)j, (seq_t)k);

        if (k == 1) {
            /* a(1) = S_1 = 1^1 = 1 */
            cache[0] = sum;
        } else {
            /* a(k) = a(k-1) * S_k */
            cache[k - 1] = cache[k - 2] * sum;
        }
    }

    return cache[n - 1];
}
