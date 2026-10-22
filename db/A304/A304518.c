/*
 * A304518  a(n) = 68*2^n - 50 (n>=1).
 * https://oeis.org/A304518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*2^n - 50 (n>=1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Computes the nth term of A304518: a(n) = 68 * 2^n - 50, for n >= 1 */
    seq_t pow_val = pow2(n);          /* 2^n */
    seq_t result  = (seq_t)68 * pow_val - (seq_t)50;
    return result;
}
