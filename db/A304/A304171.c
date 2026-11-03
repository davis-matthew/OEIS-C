/*
 * A304171  a(n) = 87*2^n - 38 (n>=0).
 * https://oeis.org/A304171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 87*2^n - 38
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 810 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t val = pow2(n);          /* 2^n */
    val = val * (seq_t)87;        /* 87*2^n */
    val = val - (seq_t)38;        /* subtract 38 */
    return val;
}
