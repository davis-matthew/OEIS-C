/*
 * A275652  a(n) =
 *          binomial(3*n,3*n/2)*binomial(2*n,n)*binomial(5*n/2,n/2)/binomial(n,n/2).
 * https://oeis.org/A275652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n-k-1, n-k) * binomial(3*n, k)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* initial binomial(2*n-1, n) */
    seq_t binom2 = binomial(2*n - 1, n);
    seq_t binom3 = (seq_t)1;
    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* result += binom2 * binom3^2 */
        seq_t term = binom3 * binom3;
        result += binom2 * term;

        /* update for next k, except after last iteration */
        if (k == n)
            break;

        /* binom3 = binom(3*n, k+1) = binom3 * (3*n - k) / (k + 1) */
        binom3 *= (seq_t)(3*n - k);
        binom3 /= (seq_t)(k + 1);

        /* binom2 = binom(2*n - (k+1) - 1, n - (k+1))
                  = binom2 * (n - k) / (2*n - k - 1) */
        binom2 *= (seq_t)(n - k);
        binom2 /= (seq_t)(2*n - k - 1);
    }

    return result;
}
