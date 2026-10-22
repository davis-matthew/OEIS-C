/*
 * A357957  a(n) = A005259(n)^5 - A005258(n)^2.
 * https://oeis.org/A357957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( Sum_{k = 0..n} binomial(n,k)^2*binomial(n+k,k)^2 )^5 -
 *            ( Sum_{k = 0..n} binomial(n,k)^2*binomial(n+k,k) )^2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t k;
    seq_t sumB1 = 0;          /* Σ binomial(n,k)^2 * binomial(n+k,k)^2   */
    seq_t sumB2 = 0;          /* Σ binomial(n,k)^2 * binomial(n+k,k)     */
    seq_t bc1 = 1;            /* current binomial(n,k)   */
    seq_t bc2 = 1;            /* current binomial(n+k,k) */

    for (k = 0; k <= n; ++k)
    {
        seq_t term1 = bc1 * bc1;   /* bc1^2 */
        term1 *= bc2;
        term1 *= bc2;              /* bc1^2 * bc2^2 */
        sumB1 += term1;

        seq_t term2 = bc1 * bc1;   /* bc1^2 */
        term2 *= bc2;              /* bc1^2 * bc2 */
        sumB2 += term2;

        if (k < n)
        {
            /* Update for next k:
               binom(n,k+1) = binom(n,k) * (n - k) / (k + 1)   */
            bc1 = bc1 * ((seq_t)n - k);
            bc1 = bc1 / ((seq_t)(k + 1));

            /* binom(n + k + 1, k + 1) = binom(n + k, k) * (n + k + 1) / (k + 1) */
            bc2 = bc2 * ((seq_t)n + k + 1);
            bc2 = bc2 / ((seq_t)(k + 1));
        }
    }

    /* Compute b1^5 and b2^2 */
    seq_t b1_pow5 = big_pow(sumB1, (seq_t)5);
    seq_t b2_mul  = sumB2 * sumB2;   /* b2^2 */

    return b1_pow5 - b2_mul;
}
