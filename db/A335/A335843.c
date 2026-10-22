/*
 * A335843  a(n) is the number of n-digit positive integers with exactly two
 *          distinct base 10 digits.
 * https://oeis.org/A335843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 81*(2^(n-1)-1) for n >= 1, with a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t v = pow2(n - 1);   /* 2^(n-1) */
    v = v - 1;               /* minus 1 */
    return 81 * v;           /* times 81 */
}
