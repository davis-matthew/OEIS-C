/*
 * A384200  Expansion of e.g.f. -log(1 - 3*x)/(3 * (1 - x)).
 * https://oeis.org/A384200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-3)*a(n-1) - 3*(n-1)^2*a(n-2) initial values: a(0) =
 *            0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev  = (seq_t)1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = ((seq_t)(4*i - 3)) * prev;
        seq_t factor = (seq_t)(3*(i-1)*(i-1));
        seq_t term2 = factor * prev2;
        cur = term1 - term2;
        prev2 = prev;
        prev = cur;
    }
    return prev;
}
