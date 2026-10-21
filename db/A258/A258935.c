/*
 * A258935  Independence number of Keller graphs.
 * https://oeis.org/A258935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n except a(1) = 4 and a(2) = 5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;
    return pow2(n);
}
