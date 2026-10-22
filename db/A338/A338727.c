/*
 * A338727  a(n) = C(n+1)^2 - 2*C(n+1)*C(n) + C(n)^2, where C() is a Catalan
 *          number; a(0) = 1.
 * https://oeis.org/A338727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(n+1) - C(n))^2 for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t c_n   = catalan(n);
    seq_t c_np1 = catalan(n + 1);
    seq_t diff  = c_np1 - c_n;
    return diff * diff;
}
