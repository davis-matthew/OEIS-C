/*
 * A387251  a(n) = 3^n*n^4.
 * https://oeis.org/A387251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 28, u128 n <= 65,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* compute n^2 */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    /* compute n^4 */
    seq_t n_four = n_sq * n_sq;
    /* compute 3^n */
    seq_t pow3_n = pow3(n);
    /* final result */
    return pow3_n * n_four;
}
