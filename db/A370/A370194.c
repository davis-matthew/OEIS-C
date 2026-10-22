/*
 * A370194  Coefficient of x^n in the expansion of ( (1+x) * (1+x^2)^2 )^n.
 * https://oeis.org/A370194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n,k) * binomial(n,n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 directly – the loop below also does this correctly */
    idx_t half = n / 2;                /* floor(n/2) */
    seq_t sum = 0;

    /* C(2n,0) */
    seq_t binom2n = 1;
    /* j = 0 corresponds to C(n,0) */
    idx_t j = 0;
    seq_t binom_n_j = 1;               /* C(n,j) */

    for (idx_t k = 0; k <= half; ++k)
    {
        /* Add the current term */
        sum += binom2n * binom_n_j;

        /* Prepare binom2n and binom_n_j for next k */
        if (k < half)
        {
            /* C(2n, k+1) = C(2n, k) * (2n - k) / (k + 1) */
            seq_t num2n = (seq_t)(2 * n - k);
            seq_t den2n = (seq_t)(k + 1);
            binom2n = binom2n * num2n / den2n;

            /* C(n, j+2)  (j = 2k) : two exact steps */
            seq_t temp = binom_n_j * (seq_t)(n - j);
            temp /= (seq_t)(j + 1);            /* C(n, j+1) */
            temp = temp * (seq_t)(n - j - 1);
            temp /= (seq_t)(j + 2);            /* C(n, j+2) */
            binom_n_j = temp;

            j += 2;                            /* next even j */
        }
    }
    return sum;
}
