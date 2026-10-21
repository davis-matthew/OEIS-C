/*
 * A272952  Number of n X 2 0..1 arrays with exactly n+2-2 having value 1 and
 *          no three 1s forming an isosceles right triangle.
 * https://oeis.org/A272952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 3*a(n-4) + a(n-5) + a(n-6)
 *            with initial values a(1)=2, a(2)=6, a(3)=10, a(4)=21, a(5)=34,
 *            a(6)=62
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: undefined for n<1, return 0 */
    if (n == 0) return 0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of slots allocated (>=1) */
    static idx_t  comp  = 0;   /* highest n already computed */

    /* allocate or expand cache to hold at least n indices (1‑based) */
    if ((size_t)n >= cap)
    {
        size_t newcap = (size_t)n + 10;   /* add some spare space */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute any missing entries */
    for (idx_t i = comp + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)          val = 2;
        else if (i == 2)     val = 6;
        else if (i == 3)     val = 10;
        else if (i == 4)     val = 21;
        else if (i == 5)     val = 34;
        else if (i == 6)     val = 62;
        else
        {
            /* Reorder arithmetic to avoid unsigned underflow */
            seq_t t = cache[i-1] + 3 * cache[i-2];
            t += cache[i-5] + cache[i-6];
            t -= 2 * cache[i-3];
            t -= 3 * cache[i-4];
            val = t;
        }
        cache[i] = val;
    }

    comp = n;               /* newly computed maximum */
    return cache[n];
}
