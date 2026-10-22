/*
 * A370669  Expansion of Sum_{k>=0} k! * ( x/(1+x^2) )^k.
 * https://oeis.org/A370669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-4)*a(n-3) + a(n-4) (n > 4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=20 Computes values in O(n)
 *            time per call, memoising all computed terms so that calls are
 *            fast even when ordered arbitrarily
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values */
    static idx_t  size    = 0;      /* allocated capacity */
    static idx_t  last    = 0;      /* largest index computed */

    /* initialise cache with first five values */
    if (cache == NULL) {
        size = 5;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL)          /* out of memory – return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)20;
        last    = 4;
    }

    /* return cached value if already known */
    if (n <= last)
        return cache[n];

    /* grow cache if necessary */
    if ((idx_t)n + 1 > size) {
        idx_t new_size = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)           /* out of memory – return 0 */
            return (seq_t)0;
        cache = tmp;
        size  = new_size;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* terms i ≤ 4 are already set */
        if (i > 4) {
            seq_t val = ((seq_t)i)           * cache[i - 1]
                      + ((seq_t)(i - 4)) * cache[i - 3]
                      +                                cache[i - 4];
            cache[i] = val;
        }
    }
    last = n;
    return cache[n];
}
