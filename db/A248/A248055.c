/*
 * A248055  G.f.: Sum_{n>=0} x^n / (1-4*x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          x^k] * [Sum_{k=0..n} C(n,k)^2 * 4^k * x^k].
 * https://oeis.org/A248055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A248055
   a(n) = Sum_{k=0..floor(n/2)} Sum_{j=0..n-2k}
           C(n-k, k+j)^2 * C(k+j, j)^2 * 4^j
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; 2 * k <= n; ++k) {
        /* start = n-2k, used as the upper part of the first binomial */
        const idx_t start = n - 2 * k;

        /* binom1 = C(n-k , k)  (initial value for j = 0) */
        seq_t binom1 = 1;
        for (idx_t i = 1; i <= k; ++i) {
            binom1 = binom1 * (seq_t)(start + i) / (seq_t)i;
        }

        /* binom2 = C(k+0 , 0) = 1  (initial value for j = 0) */
        seq_t binom2 = 1;
        seq_t power4 = 1;

        const idx_t max_j = n - 2 * k;
        for (idx_t j = 0; j <= max_j; ++j) {
            /* term = C(n-k, k+j)^2 * C(k+j, j)^2 * 4^j */
            seq_t term = binom1 * binom1 * binom2 * binom2 * power4;
            result += term;

            if (j < max_j) {
                /* update binom1 to C(n-k , k+j+1) */
                binom1 = binom1 * (seq_t)(start - j) / (seq_t)(k + j + 1);

                /* update binom2 to C(k+j+1 , j+1) */
                binom2 = binom2 * (seq_t)(k + j + 1) / (seq_t)(j + 1);

                /* multiply power4 by 4 */
                power4 <<= 2;
            }
        }
    }
    return result;
}
