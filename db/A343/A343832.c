/*
 * A343832  a(n) = Sum_{k=0..n} k! * binomial(n,k) * binomial(2*n+1,k).
 * https://oeis.org/A343832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * binomial(n,k) * binomial(2*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    seq_t result = 0;                          /* running sum a(n)   */
    seq_t fact_k = 1;                          /* k!  0! = 1         */
    seq_t binom_nk = 1;                        /* C(n,0) = 1         */
    seq_t binom_2nk = 1;                       /* C(2n+1,0) = 1      */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = k! * C(n,k) * C(2n+1,k)   */
        seq_t term = fact_k * binom_nk * binom_2nk;
        result += term;

        if (k == n) break;                     /* last term processed */

        /* update factorial: (k+1)!   */
        fact_k = fact_k * (seq_t)(k + 1);

        /* update C(n,k+1)   */
        binom_nk = (binom_nk * (seq_t)(n - k)) / (k + 1);

        /* update C(2n+1,k+1) */
        binom_2nk = (binom_2nk * (seq_t)(2 * n + 2 - (k + 1))) / (k + 1);
    }

    return result;
}
