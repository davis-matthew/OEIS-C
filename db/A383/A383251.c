/*
 * A383251  Short leg of the unique primitive Pythagorean triple whose
 *          inradius is A000108(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A383251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Catalan(n) + 1 = 2 * A000108(n) + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   _catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_catalan.h"

seq_t a(idx_t n)
{
    seq_t c = _catalan(n);
    return c * 2 + 1;
}
