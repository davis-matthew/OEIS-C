/*
 * A328875  Constant term in the expansion of (-1 + (1 + w + 1/w) * (1 + x +
 *          1/x) * (1 + y + 1/y) * (1 + z + 1/z))^n.
 * https://oeis.org/A328875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^{n-k} * binomial(n,k) * A002426(k)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002426(k) is the central trinomial coefficient with recurrence: */
    /* T0 = 1,  T1 = 1,  (k+1) * T_{k+1} = (2k+1) * T_k + 3k * T_{k-1}   */

#include <stdint.h>

/* Compute the n-th term of sequence A328875.               */
seq_t a(idx_t n)
{
    /* running binomial coefficient C(n,k)   */
    seq_t bin = 1;

    /* running central trinomial coefficients */
    seq_t T_prev2 = 1;   /* T_{k-2}  */
    seq_t T_prev1 = 1;   /* T_{k-1}  */
    seq_t T_curr  = 1;   /* T_k      */

    /* partial sums with positive/negative sign */
    seq_t pos = 0;
    seq_t neg = 0;

    /* k = 0 */
    seq_t T2   = T_curr * T_curr;       /* T(0)^2   */
    seq_t T4   = T2 * T2;               /* T(0)^4   */
    seq_t term = bin * T4;              /* bin( n,0) * T(0)^4 */

    if (((n - 0) & 1) == 0)
        pos += term;
    else
        neg += term;

    /* loop over k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* update binomial coefficient C(n,k) */
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;

        /* update central trinomial coefficient T(k) */
        if (k == 1)
        {
            T_curr = 1;    /* T(1) = 1 */
        }
        else
        {
            /* recurrence: (k) * T_k = (2(k-1)+1)*T_{k-1} + 3(k-1)*T_{k-2} */
            seq_t num = ((seq_t)(2 * (k - 1) + 1)) * T_prev1;
            num += ((seq_t)(3 * (k - 1))) * T_prev2;
            T_curr = num / (seq_t)k;

            /* shift for next iteration */
            T_prev2 = T_prev1;
            T_prev1 = T_curr;
        }

        /* compute T_k^4 */
        T2   = T_curr * T_curr;
        T4   = T2 * T2;
        term = bin * T4;

        /* add with appropriate sign */
        if (((n - k) & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    /* a(n) = non‑negative result */
    return pos - neg;
}
