/*
 * A244755  a(n) = Sum_{k=0..n} C(n,k) * (1 + 3^k)^(n-k).
 * https://oeis.org/A244755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (1 + 3^k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom   = 1;   /* C(n,0) */
    seq_t pow3    = 1;   /* 3^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base  = pow3 + 1;                /* 1 + 3^k */
        seq_t term  = seq_pow(base, n - k);    /* (1 + 3^k)^(n-k) */
        term        = term * binom;            /* C(n,k) * term */
        result      = result + term;           /* accumulate */

        /* update binomial coefficient and 3^k for next k */
        if (k < n)
        {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            pow3  = pow3 * 3;
        }
    }
    return result;
}
