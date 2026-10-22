/*
 * A370242  Coefficient of x^n in the expansion of ( 1/(1-x)^2 * (1+x^2) )^n.
 * https://oeis.org/A370242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(3*n-2*k-1,
 *            n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t   maxk = n / 2;

    /* binomial(n,0) = 1 */
    seq_t binom_nk = (seq_t)1;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* binomial(3*n-2*k-1, n-2*k) */
        seq_t Bk = binomial(3 * n - 2 * k - 1, n - 2 * k);

        /* add the term */
        sum += binom_nk * Bk;

        if (k == maxk)
            break;

        /* update binomial(n, k+1) from binomial(n, k) */
        binom_nk = (binom_nk * (seq_t)(n - k)) / (seq_t)(k + 1);
    }

    return sum;
}
