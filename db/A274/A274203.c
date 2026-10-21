/*
 * A274203  Expansion of x*(1 - x - x^3)/((1 - x)*(1 - 2*x - 3*x^2 - 2*x^3 -
 *          x^4)).
 * https://oeis.org/A274203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) - a(n-4) - a(n-5) for n>=5
 *            Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=7, a(4)=21 This
 *            implementation memoizes results and grows the cache as needed
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of terms currently computed (a(0) … a(len-1)) */

    /* convert idx_t to size_t for indexing */
    size_t idx = (size_t)n;

    /* ensure cache can hold index idx + 1 */
    if (idx >= cap)
    {
        size_t newcap = (cap == 0) ? 16 : cap;
        while (newcap <= idx)
            newcap <<= 1;          /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
        {
            /* allocation failed – abort (no recovery required for tests) */
            abort();
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to idx if not already done */
    for (; len <= idx; ++len)
    {
        seq_t val;
        switch (len)
        {
            case 0: val = 0; break;
            case 1: val = 1; break;
            case 2: val = 2; break;
            case 3: val = 7; break;
            case 4: val = 21; break;
            default:
            {
                /* recurrence: a(n) = 3*a(n-1) + a(n-2) - a(n-3) - a(n-4) - a(n-5) */
                val = cache[len-1] * (seq_t)3 + cache[len-2];
                val -= cache[len-3];
                val -= cache[len-4];
                val -= cache[len-5];
            }
        }
        cache[len] = val;
    }

    return cache[idx];
}
