/*
 * A323702  a(n) = Product_{k=0..n} (k! + (n-k)!).
 * https://oeis.org/A323702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0}^{n} (k! + (n-k)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static cache of factorials up to the greatest n seen so far */
    static seq_t *fac = NULL;
    static idx_t fac_cap = 0;

    /* enlarge the cache if necessary */
    if (fac_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_fac = NULL;
#ifdef __cplusplus
        new_fac = new seq_t[new_cap];
#else
        new_fac = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_fac)            /* out of memory */
            return 0;            /* sentinel – never expected for the tests */

        /* copy old factorial values */
        if (fac) {
            for (idx_t i = 0; i < fac_cap; ++i)
                new_fac[i] = fac[i];
#ifdef __cplusplus
            delete[] fac;
#else
            free(fac);
#endif
        }

        fac = new_fac;
        fac_cap = new_cap;

        /* compute factorials up to n */
        fac[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
    }

    /* compute the product a(n) */
    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = fac[k] + fac[n - k];
        prod *= term;
    }
    return prod;
}
