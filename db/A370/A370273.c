/*
 * A370273  Coefficient of x^n in the expansion of 1/( (1-x)^2 * (1-x^3) )^n.
 * https://oeis.org/A370273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *            binomial(3*n-3*k-1,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t k_max = n / 3;

    /* A_k = binomial(n+k-1,k)  ;  A_0 = 1 */
    seq_t A = 1;

    /* B_k = binomial(3*n-3*k-1 , n-3*k)  ;  B_0 = binomial(3*n-1 , n) */
    seq_t B = binomial(3 * n - 1, n);

    for (idx_t k = 0; k <= k_max; ++k)
    {
        /* Add current term to the sum */
        sum += A * B;

        /* No further updates after the last term */
        if (k == k_max)
            break;

        /* Update A for the next k:
           A_{k+1} = A_k * (n + k) / (k + 1) */
        A = (A * ((seq_t)n + (seq_t)k)) / ((seq_t)(k + 1));

        /* Update B for the next k:
           B_{k+1} = B_k * r / m * (r-1)/(m-1) * (r-2)/(m-2)
           where r = n - 3*k,   m = 3*n - 3*k - 1 */
        idx_t r = n - 3 * k;
        idx_t m = 3 * n - 3 * k - 1;

        B = (B * (seq_t)r) / (seq_t)m;
        B = (B * (seq_t)(r - 1)) / (seq_t)(m - 1);
        B = (B * (seq_t)(r - 2)) / (seq_t)(m - 2);
    }

    return sum;
}
