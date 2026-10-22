/*
 * A383703  Coefficient of x^3 in expansion of (x+3) * (x+7) * ... *
 *          (x+4*n-1).
 * https://oeis.org/A383703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^3 in ∏_{k=1}^n (x + (4k-1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t c0 = 1;
    seq_t c1 = 0;
    seq_t c2 = 0;
    seq_t c3 = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak = (seq_t)(4 * k - 1);

        seq_t nc3 = c2 + c3 * ak;
        seq_t nc2 = c1 + c2 * ak;
        seq_t nc1 = c0 + c1 * ak;
        seq_t nc0 = c0 * ak;

        c0 = nc0;
        c1 = nc1;
        c2 = nc2;
        c3 = nc3;
    }

    return c3;
}
