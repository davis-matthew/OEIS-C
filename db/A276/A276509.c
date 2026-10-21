/*
 * A276509  Numbers k in base 10 such that the digits of 2 + k are the digits
 *          of 2k written in reverse order.
 * https://oeis.org/A276509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*10^(n-1) - 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined below offset, return 0 */
    return (seq_t)5 * pow10(n - 1) - (seq_t)3;
}
