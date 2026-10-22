/*
 * A354973  a(0)=0; for n > 0, a(n) = 2*a(n-1) if n-1 is prime, a(n-1) + 1
 *          otherwise.
 * https://oeis.org/A354973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354973:  a(0)=0; for n>0, a(n) = 2*a(n-1) if (n-1) is prime,
   otherwise a(n-1)+1.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* recursively maintain a cache of all computed values */
    static seq_t *cache = NULL;
    static idx_t   cap   = 0;   /* number of elements stored */

    /* extend the cache if necessary */
    if (cap <= n)
    {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;  /* allocation failed – graceful fallback */

        /* copy previous entries if any */
        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];

        /* initialise new entries */
        if (cap == 0)
        {
            /* a(0) is the first term */
            new_cache[0] = 0;
            /* compute values from 1 up to new_cap-1 */
            for (idx_t i = 1; i < new_cap; ++i)
            {
                seq_t prev = new_cache[i - 1];
                if (is_prime(i - 1))
                    new_cache[i] = prev * 2;
                else
                    new_cache[i] = prev + 1;
            }
        }
        else
        {
            /* compute values from cap up to new_cap-1 */
            for (idx_t i = cap; i < new_cap; ++i)
            {
                seq_t prev = new_cache[i - 1];
                if (is_prime(i - 1))
                    new_cache[i] = prev * 2;
                else
                    new_cache[i] = prev + 1;
            }
        }

        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n];
}
