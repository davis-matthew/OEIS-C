/*
 * A365119  G.f. satisfies A(x) = (1 + x / (1 - x*A(x)))^3.
 * https://oeis.org/A365119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*(n-k+1), k) * binomial(n-1,
 *            n-k)/(n-k+1) (derived from the g.f. A(x) = (1 + x/(1 -
 *            x*A(x)))^3 with s=1, t=3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t sum  = 0;               /* result accumulator */
    idx_t m    = n;               /* current position in the summation */

    /* Start from the last term (m = n), where  
       B = binom(3*n, 1) = 3*n,   C = binom(n-1, n-1) = 1  */
    seq_t B = (seq_t)3 * (seq_t)n;
    seq_t C = (seq_t)1;

    while (m > 0) {
        /* term = B(m) * C(m) / m  */
        seq_t term = (B * C) / (seq_t)m;
        sum += term;

        if (m == 1)
            break;               /* finished all terms */

        /* Prepare for the next iteration (m -> m-1) */

        /* Ratio for B_{m-1} = B_m * R_dec  */
        seq_t a_val = (seq_t)3 * (seq_t)m;            /* a = 3*m  */
        seq_t k_val = (seq_t)(n - m + 1);             /* k = n - m + 1  */
        seq_t d_val = a_val - k_val;                 /* d = a - k  */
        seq_t numB  = d_val * (d_val - 1) * (d_val - 2) * (d_val - 3);
        seq_t denB  = (k_val + 1) * a_val * (a_val - 1) * (a_val - 2);
        B = (B * numB) / denB;

        /* Ratio for C_{m-1} = C_m * (m-1)/(n-m+1)  */
        seq_t numC = (seq_t)(m - 1);
        seq_t denC = k_val;                       /* k = n - m + 1 */
        C = (C * numC) / denC;

        --m;
    }

    return sum;
}
