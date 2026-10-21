/*
 * A259821  a(n) = floor( (3^n+1)^2/3^n ).
 * https://oeis.org/A259821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3^n + 2 for n > 0; a(0) = 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)4;
    }
    /* For n > 0: a(n) = 3^n + 2 */
    return pow3(n) + (seq_t)2;
}
