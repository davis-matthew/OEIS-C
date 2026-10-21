/*
 * A278429  a(n) = Sum_{k=0..n} binomial(k+n-2,k)*binomial(2*n+1,k+n+1).
 * https://oeis.org/A278429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+n-2,k)*binomial(2*n+1,k+n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* B_k = binomial(k+n-2,k)  (B_0 = 1) */
    seq_t B = 1;

    /* C_initial = binomial(2*n+1, n+1)  (C_0) */
    seq_t C = binomial(2*n + 1, n + 1);

    /* Sum of the products of corresponding B_k and C_k   */
    seq_t sum = B * C;          /* k = 0 term */

    for (idx_t k = 0; k < n; ++k)
    {
        /* Update B_{k+1} = B_k * (k+n-1)/(k+1)      */
        seq_t num1 = (seq_t)(k + n - 1);
        seq_t den1 = (seq_t)(k + 1);
        B = B * num1;
        B = B / den1;

        /* Update C_{k+1} = C_k * (n-k)/(k+n+2)     */
        seq_t num2 = (seq_t)(n - k);
        seq_t den2 = (seq_t)(k + n + 2);
        C = C * num2;
        C = C / den2;

        /* Add the new term to the sum */
        sum += (B * C);
    }

    return sum;
}
