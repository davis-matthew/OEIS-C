/*
 * A352651  a(n) = ( binomial(5*n,2*n)*binomial(5*n/2,2*n)*binomial(2*n,n)^2 )
 *          / binomial(5*n/2,n)^2.
 * https://oeis.org/A352651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * Σ_{k=0..n} (-1)^{n+k} * binomial(5*n, n-k) *
 *            binomial(3*n+k-1, k)^2 a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t five_n = 5 * n;
    seq_t sum = 0;
    seq_t C5n_r = binomial(five_n, n);   /* C(5n, n)         */
    seq_t ak = 1;                        /* C(3n-1, 0) = 1   */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak_sq = ak * ak;               /* square of A(k)   */
        seq_t term  = C5n_r * ak_sq;          /* current summand  */

        if ((n + k) & 1)
            sum -= term;    /* odd exponent → subtract   */
        else
            sum += term;    /* even exponent → add        */

        if (k < n) {
            /* update C(5n, n-k) → C(5n, n-k-1) */
            idx_t r = n - k;
            C5n_r = (C5n_r * (seq_t)r) /
                    (seq_t)(five_n - r + 1);

            /* update A(k)   → A(k+1)      */
            ak = (ak * (seq_t)(3 * n + k)) /
                 (seq_t)(k + 1);
        }
    }

    sum = sum * 3;
    return sum;
}
