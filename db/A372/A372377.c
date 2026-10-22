/*
 * A372377  Expansion of (1/x) * Series_Reversion( x * (1+x)^2 / (1+x+x^3)^3
 *          ).
 * https://oeis.org/A372377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * sum_{k=0..floor(n/3)} binomial(3*n+3,k) *
 *            binomial(n-k+1, n-3*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 3n+3 is small for all n of interest, so use idx_t arithmetic. */
    idx_t N3 = 3 * n + 3;
    idx_t maxk = n / 3;          /* floor(n/3) */

    seq_t sum = 0;
    seq_t bin1 = 1;              /* C(3n+3,0) */
    seq_t bin2 = (seq_t)(n + 1); /* C(n+1, n) = n+1 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        sum += bin1 * bin2;

        if (k == maxk) break;

        /* Update bin1 to C(3n+3, k+1) */
        seq_t num1 = (seq_t)(N3 - k);
        seq_t den1 = (seq_t)(k + 1);
        bin1 = bin1 * num1 / den1;

        /* Update bin2 to C(n-(k+1)+1, n-3*(k+1)) */
        idx_t Kk = n - 3 * k;            /* current K  = n-3k   */
        idx_t Nk = n - k + 1;            /* current N  = n-k+1   */

        /* C(Nk-1, Kk-3) from C(Nk, Kk) */
        /* numerator = Kk*(Kk-1)*(Kk-2)   denominator = Nk*(2k+3)*(2k+2) */
        seq_t num2 = (seq_t)Kk * (Kk - 1) * (Kk - 2);
        seq_t den2 = (seq_t)Nk * (2 * k + 3) * (2 * k + 2);
        bin2 = bin2 * num2 / den2;
    }

    return sum / ((seq_t)(n + 1));
}
