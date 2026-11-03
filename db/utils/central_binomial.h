/* central_binomial: binomial(2n, n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CENTRAL_BINOMIAL_H
#define OEISC_UTILS_CENTRAL_BINOMIAL_H
#include <stdint.h>

/* Central binomial coefficient binom(2k, k).  Computes
   C_k iteratively while avoiding overflow with a safe
   decomposition of the multiplication.  Works for all
   supported seq_t types. */
static seq_t central_binomial(idx_t k)
{
    if (k == 0)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(k + i);  /* (k+i) */
        seq_t den = (seq_t)i;        /*  i   */

        /* Perform the multiplication and division in a
           way that never overflows the seq_t type.   */
        seq_t q = res / den;
        seq_t r = res % den;
        res = q * num + (r * num) / den;
    }
    return res;
}

#endif
