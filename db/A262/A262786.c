/*
 * A262786  a(n) = 37^(2*n+1).
 * https://oeis.org/A262786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
seq_t a(idx_t n) {
    idx_t exp = 2*n + 1;   /* exponent 2*n+1 */
    return pow_uint(37, exp);
}
