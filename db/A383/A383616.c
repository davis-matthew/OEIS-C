/*
 * A383616  Semiperimeter of the unique primitive Pythagorean triple (a,b,c)
 *          such that (a-b+c)/2 is A000108(n) and such that its long leg and
 *          its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A383616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) * (2 * catalan(n) - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);     /* Catalan number C(n) */
    seq_t two_cat = cat * 2;    /* 2 * C(n) */
    seq_t tmp   = two_cat - 1;  /* 2*C(n) - 1 */
    return cat * tmp;           /* C(n) * (2*C(n) - 1) */
}
