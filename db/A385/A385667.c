/*
 * A385667  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n+1,k) *
 *          binomial(3*n-k,n-k).
 * https://oeis.org/A385667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(2*n + k, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  // if needed by the build; no conflict with idx_t/seq_t

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;   // binomial(2*n, 0)
    seq_t pow3 = 1;  // 3^0
    idx_t two_n = 2 * n;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = bin * pow3 */
        seq_t term = bin * pow3;
        sum += term;

        if (k < n) {
            /* Update bin to binomial(2*n + k + 1, k + 1) */
            bin = bin * ((seq_t)(two_n + k + 1)) / ((seq_t)(k + 1));
            /* Update power of 3 */
            pow3 *= 3;
        }
    }
    return sum;
}
