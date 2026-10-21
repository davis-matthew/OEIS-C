/*
 * A272913  a(n) = 3*2^(2*n-1)*(n-1)*n^3*binomial(2*n,n)^2, a closed form for
 *          a triple binomial sum involving absolute values.
 * https://oeis.org/A272913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(2*n-1)*(n-1)*n^3*binomial(2*n,n)^2 (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle offset: a(0) = 0, a(1) = 0 */
    if (n <= 1)
        return 0;

    /* Convert index to seq_t */
    seq_t sn     = (seq_t)n;
    seq_t sn3    = sn * sn * sn;
    seq_t bin    = binomial(2 * n, n);          /* binomial(2n, n) */
    seq_t res    = bin * bin;                   /* square the binomial */
    seq_t pow2e  = pow2(2 * n - 1);             /* 2^(2n-1) */

    /* Multiply according to the closed-form expression */
    res  = res * pow2e;                        /* multiply by 2^(2n-1) */
    res  = res * (seq_t)(n - 1);               /* multiply by (n-1) */
    res  = res * sn3;                          /* multiply by n^3 */
    res  = res * 3;                            /* multiply by 3 */

    return res;
}
