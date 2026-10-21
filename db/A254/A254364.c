/*
 * A254364  a(n) = 3*4^n + 10*2^n + 6*3^n + 5^n + 15.
 * https://oeis.org/A254364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^n + 10*2^n + 6*3^n + 5^n + 15
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   pow2, pow3, pow4, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/power5.h"

seq_t a(idx_t n) {
    // Use the explicit formula; all operations are unsigned,
    // so no overflow issues for the values that fit into seq_t.
    seq_t res = (seq_t)15;
    res += (seq_t)3 * pow4(n);
    res += (seq_t)10 * pow2(n);
    res += (seq_t)6 * pow3(n);
    res += power5(n);
    return res;
}
