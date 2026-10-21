/*
 * A272067  a(n) = (10^n-1)^4.
 * https://oeis.org/A272067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n-1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t base = pow10(n);
    base -= 1;
    return seq_pow(base, 4);
}
