/*
 * A285634  a(1) = 4, a(n) = Product_{d|a(n-1)} d.
 * https://oeis.org/A285634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4; a(n) = Product of the divisors of a(n‑1). For the
 *            special case a(1)=p^k (here p=2, k=2), we have a(n)=p^{b(n)}
 *            with b(1)=k and b(n)=b(n‑1)*(b(n‑1)+1)/2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;   /* undefined below the offset */

    /* Static caches for the exponent b(n) and the values a(n). */
    static seq_t *b_cache = NULL;   /* b(n) */
    static seq_t *a_cache = NULL;   /* a(n) */
    static size_t cache_size = 0;   /* largest index currently cached (1‑based) */

    /* Grow the caches to accommodate index n. */
    if ((size_t)n > cache_size)
    {
        size_t new_size = (size_t)n + 1;   /* indices 0 .. n; we ignore index 0 */

        seq_t *tmp;

        /* Reallocate the exponent cache. */
        tmp = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        b_cache = tmp;

        /* Reallocate the value cache. */
        tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        a_cache = tmp;

        /* Initialise the first term if this is the first allocation. */
        if (cache_size == 0)
        {
            b_cache[1] = (seq_t)2;                              /* b(1) = 2  (since a(1)=2^2) */
            a_cache[1] = big_pow((seq_t)2, b_cache[1]);         /* a(1)=4      */
            cache_size = 1;
        }

        /* Compute all missing terms up to n. */
        for (size_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t prev = b_cache[i - 1];
            /* b(i) = prev * (prev + 1) / 2. */
            seq_t next = (prev * (prev + (seq_t)1)) / (seq_t)2;
            b_cache[i] = next;
            a_cache[i] = big_pow((seq_t)2, next);
        }
        cache_size = (size_t)n;
    }

    return a_cache[n];
}
