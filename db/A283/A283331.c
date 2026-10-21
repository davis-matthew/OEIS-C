/*
 * A283331  a(n) = (Sum_{j=1..(K-1)/2} a(n-2*j+1)*a(n-2*j))/a(n-K) with
 *          a(1),...,a(K)=1, where K=7.
 * https://oeis.org/A283331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{j=1..3} a(n-2*j+1)*a(n-2*j)) / a(n-7) Initial
 *            values: a(1) = a(2) = … = a(7) = 1
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 14, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* allocated size of cache */

/* Return the n-th term of sequence A283331 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, return 0 */

    /* Ensure cache is large enough (indices 1 .. n inclusive) */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* allocation failed */
            cache = NULL;
            cache_cap = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first seven terms to 1 if not already set */
    for (idx_t i = 1; i <= 7 && i <= n; ++i)
        cache[i] = (seq_t)1;   /* overwrite is harmless */

    /* Compute terms from 8 up to n */
    if (n >= 8)
    {
        for (idx_t i = 8; i <= n; ++i)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 3; ++j)
            {
                idx_t i1 = i - 2 * j + 1;   /* n-2*j+1 */
                idx_t i2 = i - 2 * j;       /* n-2*j   */
                sum += cache[i1] * cache[i2];
            }
            cache[i] = sum / cache[i - 7];
        }
    }

    return cache[n];
}
