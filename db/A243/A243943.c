/*
 * A243943  a(n) = A006442(n)^2.
 * https://oeis.org/A243943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{k=0..n} 2^k * C(2k,k) * C(n+k,n‑k) )^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum  = 1;   /* T0 = 1 */
    seq_t term = 1;   /* current T_k */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = 2;
        num *= (seq_t)(n + k);
        num *= (seq_t)(n - k + 1);

        seq_t den = (seq_t)(k) * (seq_t)(k);

        term = term * num / den;
        sum  += term;
    }
    return sum * sum;
}
