/*
 * A367591  Expansion of 1/((1-x) * (1-3*x)^3).
 * https://oeis.org/A367591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n^2+4*n+3) * 3^(n+1) - 1)/8
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Convert index n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* Compute (2*n^2 + 4*n + 3) */
    seq_t term = 2 * nn * nn + 4 * nn + (seq_t)3;
    /* Compute 3^(n+1) using provided utility */
    seq_t pow3 = seq_pow((seq_t)3, (idx_t)(n + 1));
    /* Multiply the two terms */
    seq_t prod = term * pow3;
    /* Subtract 1 and divide by 8 */
    seq_t result = (prod - (seq_t)1) / (seq_t)8;
    return result;
}
