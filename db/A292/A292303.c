/*
 * A292303  a(1) = 1; a(n+1) = Sum_{k=1..n} lcm(a(k),n)/n.
 * https://oeis.org/A292303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1;
   a(n) = sum_{k=1}^{n-1} a(k)/gcd(a(k), n-1)   for n > 1   (OEIS A292303) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;  /* cache[i] holds a(i) */
    static idx_t  size    = 0;     /* allocated size of cache */
    static idx_t  last    = 0;     /* highest index computed */

    if (n < 1)
    {
        return 0;  /* undefined for n < 1, but input is always >=1 */
    }

    /* grow the cache if necessary */
    if (size < n + 1)
    {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        size = n + 1;
    }

    /* compute any missing terms */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            cache[1] = (seq_t)1;
        }
        else
        {
            seq_t sum = (seq_t)0;
            seq_t prev = (seq_t)(i - 1);   /* n-1 in the formula */
            for (idx_t k = 1; k < i; ++k)
            {
                seq_t g   = gcd(cache[k], prev);
                seq_t term = cache[k] / g;
                sum += term;
            }
            cache[i] = sum;
        }
    }
    last = n;
    return cache[n];
}
