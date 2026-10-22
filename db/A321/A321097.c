/*
 * A321097  Continued fraction expansion of the constant z that satisfies:
 *          CF(6*z, n) = CF(z, n) + 55, for n >= 0, where CF(z, n) denotes the
 *          n-th partial denominator in the continued fraction expansion of z.
 * https://oeis.org/A321097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 10, a(3k) = 9 (k ≥ 1) a(3k+2) = 6 – a(3k+1) a(9k+1) = 1
 *            a(9k+7) = 5 a(9k+4) = 6 – a(3k+1) (this is a(3(3k+1)+1))
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A321097 (0‑based). */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size)
    {
        /* grow cache to hold indices 0 … n */
        idx_t new_size = n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_size);
        if (cache != NULL)
        {
            /* copy old contents */
            for (idx_t i = 0; i < cache_size; ++i)
            {
                new_cache[i] = cache[i];
            }
            seq_free_array(cache);
        }
        cache = new_cache;
        cache_size = new_size;

        /* compute terms that are now available */
        for (idx_t i = 0; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)10;
            }
            else if (i % 3 == 0)
            {
                cache[i] = (seq_t)9;
            }
            else if (i % 3 == 1)
            {
                if (i % 9 == 1)
                {
                    cache[i] = (seq_t)1;
                }
                else if (i % 9 == 7)
                {
                    cache[i] = (seq_t)5;
                }
                else   /* i % 9 == 4 */
                {
                    /* compute the corresponding m = 3*k+1  with k = (i-4)/9 */
                    idx_t m = (i - 4) / 3 + 1;
                    cache[i] = (seq_t)6 - cache[m];
                }
            }
            else   /* i % 3 == 2 */
            {
                cache[i] = (seq_t)6 - cache[i - 1];
            }
        }
    }
    return cache[n];
}
