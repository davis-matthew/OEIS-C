/*
 * A254707  Expansion of (1 + 2*x^2) / ((1 - x^2)^2 * (1 - x^3) * (1 - x^4))
 *          in powers of x.
 * https://oeis.org/A254707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254707
    G.f.: (1 + 2*x^2) / ((1 - x^2)^2 * (1 - x^3) * (1 - x^4))
    Recurrence (valid for all n ≥ 0):
        a(n+10) = a(n) + a(n+1) - a(n+2) - 2*a(n+3) - 2*a(n+4)
                  + 2*a(n+6) + 2*a(n+7) + a(n+8) - a(n+9) + 3
    Initial values:
        a(0)=1, a(1)=0, a(2)=4, a(3)=1, a(4)=8,
        a(5)=4, a(6)=15, a(7)=8, a(8)=25, a(9)=15
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static idx_t max_comp   = 9;   /* largest index whose value is known */

    /* first call – initialise cache with at least 10 elements */
    if (cache == NULL)
    {
        /* require space for the requested index */
        size_t need = (size_t)n + 1;
        if (need < 10) need = 10;
        cache_cap = need;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));

        /* initial values up to a(9) */
        static const seq_t init[10] = {1, 0, 4, 1, 8, 4, 15, 8, 25, 15};
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init[i];
    }
    else if ((size_t)n + 1 > cache_cap)   /* enlarge cache if necessary */
    {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        /* new entries are uninitialised – they will be filled during the loop */
    }

    /* compute values up to n if needed */
    if (n > max_comp)
    {
        for (idx_t j = max_comp + 1; j <= n; ++j)
        {
            /* compute a(j) = a(j‑10)+a(j‑9)-a(j‑8)-2*a(j‑7)-2*a(j‑6)
               +2*a(j‑4)+2*a(j‑3)+a(j‑2)-a(j‑1)+3
               Reorder to avoid underflow:
               pos = a(j‑10)+a(j‑9)+2*a(j‑4)+2*a(j‑3)+a(j‑2)+3
               neg = a(j‑8)+2*a(j‑7)+2*a(j‑6)+a(j‑1)
            */
            seq_t pos = cache[j-10] + cache[j-9] + cache[j-4]*(seq_t)2
                      + cache[j-3]*(seq_t)2 + cache[j-2] + (seq_t)3;
            seq_t neg = cache[j-8] + cache[j-7]*(seq_t)2 + cache[j-6]*(seq_t)2
                      + cache[j-1];
            cache[j] = pos - neg;
        }
        max_comp = n;
    }

    return cache[n];
}
