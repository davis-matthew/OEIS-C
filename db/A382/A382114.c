/*
 * A382114  Semiperimeter of the unique primitive Pythagorean triple whose
 *          inradius is A000108(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (catalan(n) + 1)*(2*catalan(n) + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c = catalan(n);
    seq_t t1 = c + 1;      /* c + 1 */
    seq_t t2 = c * 2 + 1;  /* 2*c + 1 */
    return t1 * t2;        /* (c + 1)*(2*c + 1) */
}
