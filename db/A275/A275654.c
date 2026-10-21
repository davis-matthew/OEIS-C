/*
 * A275654  a(n) = (5*n)!/((3*n)!*n!^2) * ((3*n/2)!*(7*n/2)!)/(5*n/2)!^2.
 * https://oeis.org/A275654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(4*n-k-1, n-k) * binomial(5*n, k)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    idx_t five_n = 5 * n;          /* 5*n, used for updating binomial(5n,k) */
    seq_t sum     = 0;             /* running total */

    /* binomial(4*n-1, n)  ->  C(4n-1, n), term for k = 0 */
    seq_t binom4 = binomial(4 * n - 1, n);   /* C(4n-1, n)   */
    seq_t binom5k = 1;                     /* binomial(5n, 0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = C(4n-k-1, n-k) * C(5n, k)^2 */
        seq_t term = binom4 * binom5k * binom5k;
        sum += term;

        if (k == n)   /* finished all terms */
            break;

        /* update C(5n, k+1) = C(5n, k) * (5n-k) / (k+1) */
        binom5k = binom5k * (seq_t)(five_n - k) / (k + 1);

        /* update C(4n-(k+1)-1, n-(k+1))
           = C(4n-k-1, n-k) * (n-k) / (4n-k-1) */
        binom4 = binom4 * (seq_t)(n - k) /
                 (seq_t)(4 * n - k - 1);
    }

    return sum;
}
