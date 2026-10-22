/*
 * A320661  a(n) = 2^(n+3) - 6*n - 7.
 * https://oeis.org/A320661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3) - 6*n - 7
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t val = pow2(n + 3);            /* 2^(n+3) */
    seq_t mul = (seq_t)6 * (seq_t)n;    /* 6*n */
    val -= mul;                         /* 2^(n+3) - 6*n */
    val -= (seq_t)7;                    /* 2^(n+3) - 6*n - 7 */
    return val;
}
