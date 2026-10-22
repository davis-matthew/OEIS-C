/*
 * A361595  Expansion of e.g.f. exp( (x / (1-x))^3 ) / (1-x).
 * https://oeis.org/A361595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361595
   Recurrence:
   a(0)=1, a(1)=1, a(2)=2, a(3)=12
   a(n) = (4*n-3)*a(n-1)
          -3*(n-1)*(2*n-3)*a(n-2)
          +2*(n-2)*(n-1)*(2*n-3)*a(n-3)
          -(n-3)^2*(n-2)*(n-1)*a(n-4)
   (for n>=4)
   See OEIS entry for derivation.
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values   */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t used  = 0;      /* number of entries computed */

    /* initialise base terms on first use */
    if (used == 0)
    {
        size_t init = n + 1;
        if (init < 4) init = 4;            /* at least 4 terms */
        cache = (seq_t*)realloc(cache, init * sizeof(seq_t));
        for (size_t i = 0; i < init; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)12;
        cap  = init;
        used = 4;                          /* values 0..3 known */
    }

    /* extend cache and compute missing values up to n */
    if ((size_t)n >= used)
    {
        if ((size_t)n + 1 > cap)
        {
            size_t new_cap = (size_t)n + 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            for (size_t i = cap; i < new_cap; ++i)
                cache[i] = (seq_t)0;
            cap = new_cap;
        }

        for (idx_t i = used; i <= n; ++i)
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];

            seq_t term = ((seq_t)(4 * i - 3)) * a1;
            term -= ((seq_t)3 * (i - 1) * (seq_t)(2 * i - 3)) * a2;
            term += ((seq_t)2 * (i - 2) * (i - 1) * (seq_t)(2 * i - 3)) * a3;
            seq_t im3 = (seq_t)(i - 3);
            term -= im3 * im3 * (i - 2) * (i - 1) * a4;

            cache[i] = term;
        }
        used = (size_t)n + 1;
    }

    return cache[n];
}
