/*
 * A392032  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) * binomial(n,k) *
 *          binomial(n-k,k).
 * https://oeis.org/A392032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) * binomial(n,k) *
 *            binomial(n-k,k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t B1 = 1; /* C(n,0) */
    seq_t B2 = 1; /* C(n,0) */

    for (idx_t k = 0; k <= n / 2; ++k) {
        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t tri = ((seq_t)(k + 2) * (k + 1)) / 2;
        seq_t term = tri * B1 * B2;
        result += term;

        if (k == n / 2)
            break; /* last iteration */

        /* Update C(n,k+1) */
        seq_t num1 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(k + 1);
        B1 = B1 * num1 / den1;

        /* Update C(n-k-1,k+1) = C(n-(k+1),k+1) */
        seq_t num2 = ((seq_t)(n - 2 * k)) * ((seq_t)(n - 2 * k - 1));
        seq_t den2 = (seq_t)(k + 1) * (seq_t)(n - k);
        B2 = B2 * num2 / den2;
    }

    return result;
}
