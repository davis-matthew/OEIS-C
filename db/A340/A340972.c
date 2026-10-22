/*
 * A340972  a(n) = Sum_{k=0..n} (-n)^k * binomial(n,k) * binomial(2*k,k).
 * https://oeis.org/A340972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-n)^k * binomial(n,k) * binomial(2k,k)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A340972:  a(n) = Sum_{k=0..n} (-n)^k * binomial(n,k) * binomial(2*k,k). */
seq_t a(idx_t n)
{
    seq_t result = 0;           /* accumulated sum */
    seq_t bin1   = 1;           /* binomial(n,0)  */
    seq_t bin2   = 1;           /* binomial(0,0)  */
    seq_t term_power = 1;       /* (-n)^0            */
    int   sign = 1;             /* sign of (-n)^k   */
    seq_t n_seq = (seq_t)n;     /* cast n once */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = bin1 * bin2 * term_power with appropriate sign */
        seq_t term = bin1 * bin2 * term_power;
        if (sign < 0) term = -term;
        result += term;

        if (k == n) break;  /* finished all terms */

        /* update bin1 to binomial(n,k+1) */
        bin1 = bin1 * (n_seq - (seq_t)k) / ((seq_t)k + 1);

        /* update bin2 to binomial(2(k+1),k+1) */
        idx_t kp1 = k + 1;
        seq_t two_kp1 = (seq_t)kp1 * 2;
        seq_t numer = (two_kp1 - 1) * two_kp1;               /* (2k+1)(2k+2) */
        seq_t denom = (seq_t)kp1 * (seq_t)kp1;              /* (k+1)^2   */
        bin2 = bin2 * numer / denom;

        /* update (-n)^{k+1} */
        term_power *= n_seq;

        /* flip sign for next exponent */
        sign = -sign;
    }

    return result;
}
