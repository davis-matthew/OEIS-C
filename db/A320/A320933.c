/*
 * A320933  a(n) = 2^n - floor((n+3)/2).
 * https://oeis.org/A320933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - floor((n+3)/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute 2^n */
    seq_t power = pow2(n);
    /* Compute floor((n+3)/2) as an integer */
    seq_t sub = (seq_t)((n + 3) / 2);
    /* Return the difference */
    return power - sub;
}
