/*
 * A330170  a(n) = 2^n + 3^n + 6^n - 1.
 * https://oeis.org/A330170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 3^n + 6^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t term2  = pow2(n);          /* 2^n */
    seq_t term3  = pow3(n);          /* 3^n */
    seq_t term6  = pow_uint(6, n);   /* 6^n */
    seq_t sum    = term2 + term3 + term6;
    return sum - (seq_t)1;
}
