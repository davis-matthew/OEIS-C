/*
 * A302183  Number of 3D n-step walks of type abd.
 * https://oeis.org/A302183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+2)^2*a(n)+(-3*n^2-5*n-1)*a(n-1)-(13*n+15)*(n-1)*a(n-2)
                 +15*(n-1)*(n-2)*a(n-3)=0, with a(0)=1, a(1)=1, a(2)=4 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;     /* highest index in cache */

    /* initialise cache with first three values */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        max_cached = 2;
    }

    if (n <= max_cached)
        return cache[n];

    /* extend cache to at least n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        /* early values 0,1,2 are already set */
        if (i < 3)
        {
            continue;
        }

        seq_t nn   = (seq_t)i;          /* convert index to seq_t */
        seq_t a1   = cache[i - 1];
        seq_t a2   = cache[i - 2];
        seq_t a3   = cache[i - 3];

        seq_t term1, term2, term3, denom;
        /* coefficient for term1: (3*n^2 + 5*n + 1) */
        /* coefficient for term2: (13*n + 15)*(n-1) */
        /* coefficient for term3: 15*(n-1)*(n-2) */

        if (sizeof(seq_t) == 8)   /* 64‑bit build – use 128‑bit intermediate */
        {
            unsigned __int128 nn128 = (unsigned __int128)nn;
            unsigned __int128 a1_128 = (unsigned __int128)a1;
            unsigned __int128 a2_128 = (unsigned __int128)a2;
            unsigned __int128 a3_128 = (unsigned __int128)a3;

            unsigned __int128 num1 = (unsigned __int128)3 * nn128 * nn128
                                  + (unsigned __int128)5 * nn128
                                  + (unsigned __int128)1;
            unsigned __int128 num2 = (unsigned __int128)(13 * n + 15)
                                  * (nn128 - 1);
            unsigned __int128 num3 = (unsigned __int128)15
                                  * (nn128 - 1)
                                  * (nn128 - 2);

            unsigned __int128 numerator = num1 * a1_128
                                         + num2 * a2_128
                                         - num3 * a3_128;
            unsigned __int128 denominator = (nn128 + 2) * (nn128 + 2);

            cache[i] = (seq_t)(numerator / denominator);
        }
        else                      /* arbitrary‑precision build – use seq_t ops */
        {
            term1 = (( (seq_t)3 * nn * nn ) + ( (seq_t)5 * nn ) + (seq_t)1) * a1;
            term2 = (( (seq_t)(13 * n + 15) ) * (nn - (seq_t)1)) * a2;
            term3 = ((seq_t)15 * (nn - (seq_t)1) * (nn - (seq_t)2)) * a3;

            denom = (nn + (seq_t)2) * (nn + (seq_t)2);

            cache[i] = (term1 + term2 - term3) / denom;
        }
    }

    max_cached = n;
    return cache[n];
}
