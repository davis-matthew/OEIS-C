/*
 * A389225  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(4*k,n-k).
 * https://oeis.org/A389225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(4*k,n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided binomial helper and incremental
   computation of C(n,k) to keep intermediate values small. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cnk = 1; /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t rem = n - k;                     /* n-k */
        if (rem <= 4 * k)                      /* binomial(4k,n-k) is non-zero only then */
        {
            seq_t bk = binomial(4 * k, rem);   /* C(4k, n-k) */
            sum += cnk * bk;
        }

        if (k < n) /* update C(n,k+1) for next iteration */
        {
            /* cnk_{next} = cnk * (n-k) / (k+1) */
            seq_t numer = (seq_t)(n - k);
            seq_t denom = (seq_t)(k + 1);
            cnk = (cnk * numer) / denom;
        }
    }
    return sum;
}
