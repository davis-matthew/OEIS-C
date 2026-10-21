/*
 * A298611  Expansion of (1 - 6*x + x^2 - 8*x^3 + 16*x^4)^(-1/2).
 * https://oeis.org/A298611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298611: Expansion of (1 - 6*x + x^2 - 8*x^3 + 16*x^4)^(-1/2)
   Recurrence (for n ≥ 4):
       n * a(n) = -(16*n - 32) * a(n-4)
                 + (8*n  - 12) * a(n-3)
                 - (n-1)      * a(n-2)
                 + (6*n  - 3)  * a(n-1)
   Initial values:
       a(0) = 1, a(1) = 3, a(2) = 13, a(3) = 67
   The function memoises results in a static cache so that calls in any
   order are efficient.  It uses only integer operations allowed by the
   specification. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;             /* number of elements currently stored */

    if (n < cache_len)                      /* already computed */
        return cache[n];

    /* grow the cache to hold indices up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                     /* allocation failed – return 0 */

    /* old cache length determines where we start filling new slots */
    idx_t start = cache_len;
    cache = tmp;
    cache_len = new_len;

    /* compute new values as needed */
    for (idx_t i = start; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)13;
        else if (i == 3)
            cache[i] = (seq_t)67;
        else
        {
            seq_t a_n4 = cache[i - 4];
            seq_t a_n3 = cache[i - 3];
            seq_t a_n2 = cache[i - 2];
            seq_t a_n1 = cache[i - 1];

            seq_t term_n4 = (seq_t)(16 * i - 32) * a_n4;   /* (16*i-32)*a(i-4) */
            seq_t term_n3 = (seq_t)(8 * i - 12)  * a_n3;   /* (8*i-12)*a(i-3) */
            seq_t term_n2 = (seq_t)(i - 1)     * a_n2;     /*  (i-1)*a(i-2)   */
            seq_t term_n1 = (seq_t)(6 * i - 3)  * a_n1;   /* (6*i-3)*a(i-1)  */

            seq_t numerator = -term_n4 + term_n3 - term_n2 + term_n1;
            cache[i] = numerator / (seq_t)i;              /* divide by n (i) */
        }
    }

    return cache[n];
}
