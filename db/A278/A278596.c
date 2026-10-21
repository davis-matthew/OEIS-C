/*
 * A278596  a(n) = Sum_{k=0..n} binomial(k+n+3,k)*binomial(2*n+1,n-k).
 * https://oeis.org/A278596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+n+3,k) * binomial(2*n+1,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t C     = 1;                               /* binomial(n+3, n+3) */
    seq_t B     = binomial(2*n + 1, n);              /* binomial(2n+1, n) */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += C * B;                               /* add current term */

        if (k == n) break;                         /* last iteration complete */

        /* update C : binomial(k+n+4, n+3) = C * (k+n+4)/(k+1) */
        C *= (seq_t)(k + n + 4);
        C /= (seq_t)(k + 1);

        /* update B : binomial(2n+1, n-k-1) = B * (n-k)/(n+k+2) */
        B *= (seq_t)(n - k);
        B /= (seq_t)(n + k + 2);
    }

    return sum;
}
