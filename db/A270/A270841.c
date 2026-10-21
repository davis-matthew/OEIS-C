/*
 * A270841  a(1) = 5; a(n) is the sum of |a(m) - m| for m < n.
 * https://oeis.org/A270841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5 for n = 1, 4 for n = 2, 2^(n-2) + n + 1 for n > 2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)4;
    seq_t res = pow2(n - 2);
    res += (seq_t)n;
    res += (seq_t)1;
    return res;
}
