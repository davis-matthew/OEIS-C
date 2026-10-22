/*
 * A323227  a(n) = [x^n] (1 - 2*x + x^2 - 2*x^3 + x^4)/((1 - x)^2*(1 - 2*x)).
 * https://oeis.org/A323227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) + n + 1 for n ≥ 2 ; a(0) = 1 , a(1) = 2. (Uses
 *            the closed‑form from the OEIS entry.)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    return pow2(n - 2) + (seq_t)n + (seq_t)1;
}
