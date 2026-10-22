/*
 * A357711  Expansion of e.g.f. cosh( 2 * log(1-x) ).
 * https://oeis.org/A357711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-3)*a(n-1) - (n^2-4*n)*a(n-2), a(0)=1, a(1)=0, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)0;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t k = 2; k <= n; ++k) {
        if (k == 2) {
            cur = (seq_t)4;
        } else {
            seq_t term1 = (seq_t)(2 * k - 3) * prev1;
            int64_t coeff = (int64_t)k * (int64_t)(k - 4);  /* n*(n-4) = n^2-4n */
            if (coeff >= 0)
                cur = term1 - (seq_t)coeff * prev2;
            else
                cur = term1 + (seq_t)(-coeff) * prev2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
