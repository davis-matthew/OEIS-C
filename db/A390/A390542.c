/*
 * A390542  a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n+2*k+1,k).
 * https://oeis.org/A390542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n+2*k+1 , k) (OEIS
 *            A390542)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sum is split into positives and negatives to avoid
       subtracting with an unsigned type.  The final result is
       guaranteed to be positive, so sum_pos >= sum_neg. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* B_k = binomial(n+2*k+1 , k), B_0 = 1 */
    seq_t Bk = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Add Bk with correct sign */
        if (((n - k) & 1) == 0)  /* even exponent → positive term */
            sum_pos += Bk;
        else
            sum_neg += Bk;

        /* Compute B_{k+1} if we are not at the last term.  The
           recurrence B_{k+1} = B_k * (n+2k+3)(n+2k+2) / ((k+1)(n+k+2))
           is evaluated by canceling common factors before any
           multiplication to keep intermediate values as small as
           possible.  The algorithm below repeatedly reduces the
           numerator and denominator by gcds. */
        if (k == n)
            break;

        idx_t kk1 = k + 1;
        seq_t num1 = (seq_t)(n + 2 * kk1 + 1);  /* n + 2(k+1) + 1 = n + 2k + 3 */
        seq_t num2 = (seq_t)(n + 2 * kk1);      /* n + 2(k+1)   = n + 2k + 2 */
        seq_t den1 = (seq_t)kk1;                /* k + 1 */
        seq_t den2 = (seq_t)(n + kk1 + 1);      /* n + (k+1) + 1 = n + k + 2 */

        seq_t res = Bk;

        /* Cancel common factors with the current result */
        seq_t g;
        g = gcd(res, den1);
        res /= g;
        den1 /= g;
        g = gcd(res, den2);
        res /= g;
        den2 /= g;

        /* Cancel common factors between numerator terms and denominators */
        g = gcd(num1, den1);
        num1 /= g;
        den1 /= g;
        g = gcd(num1, den2);
        num1 /= g;
        den2 /= g;
        g = gcd(num2, den1);
        num2 /= g;
        den1 /= g;
        g = gcd(num2, den2);
        num2 /= g;
        den2 /= g;

        /* Remaining denominators should be 1; multiply the numerators */
        res *= num1;
        res *= num2;

        Bk = res;
    }

    return sum_pos - sum_neg;
}
