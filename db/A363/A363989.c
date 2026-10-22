/*
 * A363989  a(n) = Sum_{k = 0..n}
 *          (-4)^(n-k)*binomial(n,k)*binomial(4*n+k,k)*binomial(2*k,k).
 * https://oeis.org/A363989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n}
 *            (-4)^(n-k)*binomial(n,k)*binomial(4*n+k,k)*binomial(2*k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, mul_small, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_small.h"
#include "../utils/pow4.h"

/* Compute sequence A363989 */
seq_t a(idx_t n)
{
    seq_t sum = 0;                 /* sum of terms with even idx (positive sign)   */
    seq_t negsum = 0;              /* sum of terms with odd  idx (negative sign)  */
    seq_t pow = 1;                 /* 4^idx  (idx = n-k)                           */

    for (idx_t idx = 0; idx <= n; ++idx) {
        idx_t k = n - idx;                      /* k = n - idx                               */

        seq_t bin1 = binomial(n, k);           /* binomial(n, k)                            */
        seq_t bin2 = binomial(4 * n + k, k);   /* binomial(4n + k, k)                       */
        seq_t bin3 = binomial(2 * k, k);       /* binomial(2k, k)                           */

        seq_t term = bin1 * bin2;
        term = term * bin3;
        term = term * pow;                    /* multiply by 4^idx (sign handled later)     */

        if (idx & 1) {
            negsum += term;                   /* odd index => negative sign                */
        } else {
            sum += term;                      /* even index => positive sign               */
        }

        if (idx != n) {                      /* prepare 4^(idx+1) for next iteration      */
            pow = mul_small(pow, 4);
        }
    }

    return sum - negsum;   /* final result, sum >= negsum for all n                */
}
