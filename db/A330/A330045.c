/*
 * A330045  Expansion of e.g.f. exp(x) / (1 - x^4).
 * https://oeis.org/A330045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330045: 
   a(0)=a(1)=a(2)=a(3)=1;
   a(n)=n*(n-1)*(n-2)*(n-3)*a(n-4)+1 for n≥4
   (equivalently a(n)=∑ k=0..⌊n/4⌋ n!/(n-4k)!).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n >= cap)                    /* grow cache so that it contains a[n] */
    {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)              /* allocation failed – terminate */
            abort();
        cache = new_cache;

        for (idx_t i = cap; i <= n; ++i)
        {
            if (i < 4) 
                cache[i] = 1;
            else
            {
                seq_t prod = (seq_t)i * (i - 1);
                prod *= (i - 2);
                prod *= (i - 3);
                prod *= cache[i - 4];
                cache[i] = prod + 1;
            }
        }
        cap = new_cap;
    }
    return cache[n];
}
