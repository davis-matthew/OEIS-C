/*
 * A343787  Number of ordered partitions of an n-set without blocks of size 4.
 * https://oeis.org/A343787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343787
   Number of ordered partitions of an n‑set without blocks of size 4.
   Recurrence:
       a(0) = 1
       a(n) = Σ_{k=1..n, k≠4} C(n,k) * a(n‑k)
   where C(n,k) is the binomial coefficient.  This follows from the
   egf 1/(2 + x^4/4! – e^x).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* array of a(i) values */
    static size_t  cap    = 0;      /* allocated capacity in terms */
    static idx_t  last    = 0;      /* highest index currently computed */

    /* initialise cache for a(0)=1 on first call */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;           /* a(0)=1 */
        cap  = 1;
        last = 0;
    }

    /* expand cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed */
        /* zero new slots (not strictly necessary) */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n that are not yet computed */
    for (size_t i = (size_t)last + 1; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)i;               /* C(i,1) */
        for (size_t k = 1; k <= i; ++k) {
            if (k != 4)                      /* skip block size 4 */
                sum += binom * cache[i - k];

            if (k < i) {                     /* update to C(i,k+1) */
                /* C(i,k+1) = C(i,k) * (i-k) / (k+1) */
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        cache[i] = sum;
    }

    last = (size_t)n;
    return cache[n];
}
