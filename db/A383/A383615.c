/*
 * A383615  Length of the long leg of the unique primitive Pythagorean triple
 *          (x,y,z) such that (x-y+z)/2 = A000108(n) and its long leg and
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A383615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*C(n)*(C(n)-1) where C(n)=A000108(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if (n < 2) {
        return 0;
    }
    seq_t c = catalan(n);
    return 2 * c * (c - 1);
}
