/*
 * A367921  Expansion of e.g.f. exp(4*(exp(x) - 1) - 3*x).
 * https://oeis.org/A367921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = -3*a(n-1) + 4*Σ_{k=1..n} binom(n-1,k-1)*a(n-k)
 *            for n≥1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises values in a dynamic array so that
   calls can be made in any order.  The routine uses only integer
   arithmetic and works for any seq_t that supports the set of
   operations described in the requirements. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* computed values */
    static size_t cache_capacity = 0;    /* allocated size of cache */
    static size_t last_computed = 0;     /* highest n that has been
                                           computed so far */

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache_capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) return (seq_t)0;          /* memory error */
        cache[0] = (seq_t)1;                  /* a(0) */
        last_computed = 0;
    }

    /* resize cache if necessary */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;            /* memory error */
        cache = tmp;
        for (size_t i = cache_capacity; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_capacity = new_cap;
    }

    /* compute any values up to n that are not yet known */
    if (n > last_computed) {
        for (size_t m = last_computed + 1; m <= (size_t)n; ++m) {
            seq_t sum   = 0;
            seq_t binom = 1;                      /* binom(m-1,0) */
            sum += binom * cache[0];              /* j=0 term */

            size_t nm1 = m - 1;
            for (size_t j = 1; j <= nm1; ++j) {
                /* binom(m-1,j) = binom(m-1,j-1)*(m-1-(j-1))/j */
                binom = (binom * (seq_t)(nm1 - (j - 1))) / (seq_t)j;
                sum += binom * cache[j];
            }

            seq_t val = (seq_t)4 * sum - (seq_t)3 * cache[m - 1];
            cache[m] = val;
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
