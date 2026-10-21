/*
 * A253324  Number of (n+1)X(6+1) 0..1 arrays with every 2X2 subblock sum
 *          nondecreasing horizontally, vertically and antidiagonally
 *          ne-to-sw.
 * https://oeis.org/A253324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 20, u64 n <= 52, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253324: number of (n+1)×7 binary arrays with
   every 2×2 subblock sum non‑decreasing horizontally, vertically
   and along the NE→SW antidiagonal.
   Modeled by the linear recurrence
     a(n) = 4*a(n‑1) + 2*a(n‑2) – 26*a(n‑3) + 16*a(n‑4)
            + 62*a(n‑5) – 70*a(n‑6) – 62*a(n‑7) + 109*a(n‑8)
            + 14*a(n‑9) – 76*a(n‑10) + 16*a(n‑11) + 20*a(n‑12)
            – 8*a(n‑13)
   valid for n > 23.
   The first 23 terms are given in the OEIS entry.
   The implementation caches computed values; it is thread‑unsafe
   but meets the requirements for single‑threaded usage.
*/
#include <stdlib.h>

/* The function signature mandated by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)                /* safety for out of bounds */
        return 0;

    /* Static cache holds computed a[1..max_comp] (indices 0 unused). */
    static seq_t *cache = NULL;      /* pointer to cache array */
    static idx_t  capacity = 0;      /* allocated size (index 0 unused) */
    static idx_t  max_comp = 0;      /* largest n whose value is known */

    /* Ensure we have at least enough room for n + 1 elements. */
    if (capacity <= n)
    {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < (size_t)(23 + 1))    /* always keep room for the 23 seeds */
            new_cap = 24;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        capacity = (idx_t)new_cap;
        /* initialise new part to zero (only needed for very big n) */
        for (size_t i = max_comp + 1; i < capacity; ++i)
            cache[i] = 0;
    }

    /* First time: load the 23 known terms.  We always load all 23,
       even if the current n is smaller, because the recurrence needs
       the last 13 terms when n exceeds 23. */
    if (max_comp == 0)
    {
        static const seq_t init[24] = {
            0ULL, /* unused index 0 */
            1482ULL, 6936ULL, 31999ULL, 66196ULL, 173903ULL,
            283804ULL, 600020ULL, 952108ULL, 1887609ULL,
            3123964ULL, 6138900ULL, 10795040ULL, 21356925ULL,
            39547774ULL, 78761338ULL, 150942126ULL, 301625253ULL,
            589680938ULL, 1179726447ULL, 2331440768ULL,
            4665269359ULL, 9272462376ULL, 18552625342ULL
        };
        for (idx_t i = 1; i <= 23; ++i)
            cache[i] = init[i];
        max_comp = 23;
    }

    /* If the requested n is beyond what we have computed,
       use the recurrence to extend the cache. */
    if (n > max_comp)
    {
        /* Coefficients for a(n) in terms of a(n-1)..a(n-13). */
        const int coeff[13] = {
            4,  2, -26, 16, 62, -70, -62, 109,
            14, -76, 16, 20, -8
        };

        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t pos = 0, neg = 0;
            for (int j = 0; j < 13; ++j)
            {
                idx_t k = i - (j + 1);           /* i-1 .. i-13 */
                seq_t t = cache[k];
                int c = coeff[j];
                if (c > 0)
                    pos += t * (seq_t)c;
                else
                    neg += t * (seq_t)(-c);
            }
            cache[i] = pos - neg;
        }
        max_comp = n;
    }

    return cache[n];
}
