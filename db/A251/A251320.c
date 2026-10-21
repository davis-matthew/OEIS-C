/*
 * A251320  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          having one or two 1s.
 * https://oeis.org/A251320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) - 13*a(n-3) - a(n-4) + 2*a(n-5) with
 *            initial values: a(1) = 28, a(2) = 116, a(3) = 474, a(4) = 1982,
 *            a(5) = 8218 (for n ≤ 5 the explicit values are used, for n > 5
 *            the recurrence applies)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;           /* allocated size (indices 0..cap-1) */
    static idx_t  current_n = 0;           /* highest n for which cache[n] is valid */

    /* grow cache if needed */
    if (n > current_n)
    {
        /* allocate or resize buffer */
        if (cache == NULL)
        {
            cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
            cache_cap = n + 1;
        }
        else if (cache_cap < n + 1)
        {
            seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
            if (tmp == NULL) return 0;      /* allocation failure – not expected */
            cache = tmp;
            cache_cap = n + 1;
        }

        /* initialise missing entries */
        for (idx_t i = current_n + 1; i <= n; ++i)
        {
            if (i <= 5) /* explicit initial values */
            {
                switch (i)
                {
                    case 1: cache[1] = 28;   break;
                    case 2: cache[2] = 116;  break;
                    case 3: cache[3] = 474;  break;
                    case 4: cache[4] = 1982; break;
                    case 5: cache[5] = 8218; break;
                }
            }
            else /* recurrence for i > 5 */
            {
                cache[i] =
                    3 * cache[i-1]          /* 3*a(n-1) */
                  + 8 * cache[i-2]          /* +8*a(n-2) */
                  - 13 * cache[i-3]         /* -13*a(n-3) */
                  -     cache[i-4]          /* -a(n-4)   */
                  + 2 * cache[i-5];         /* +2*a(n-5) */
            }
        }
        current_n = n;
    }
    return cache[n];
}
