/*
 * A373683  Expansion of e.g.f. exp(x / (1 - x^2)) / (1 - x^2).
 * https://oeis.org/A373683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} binomial(n-k,k)/(n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* fast direct computation using a recurrence for the summand */
    idx_t maxk = n / 2;
    seq_t sum = (seq_t)0;
    seq_t C   = (seq_t)1;  /* binom(n-k,k) */
    seq_t P   = (seq_t)1;  /* (n-2k+1)*...*n   */

    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0) {
            /* update C: C = C * (n-2k+2)*(n-2k+1) / k / (n-k+1)  */
            seq_t num1 = (seq_t)(n - 2*k + 2);
            seq_t num2 = (seq_t)(n - 2*k + 1);
            C = C * num1 * num2;
            C /= (seq_t)k;
            C /= (seq_t)(n - k + 1);

            /* update P: multiply the two new factors for the product range */
            seq_t f1 = (seq_t)(n - 2*k + 1);
            seq_t f2 = (seq_t)(n - 2*k + 2);
            P *= f1 * f2;
        }

        /* add term_k = C * P to the sum */
        sum += C * P;
    }

    return sum;
}
