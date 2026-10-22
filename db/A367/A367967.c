/*
 * A367967  a(n) = 3/4*(3^(n + 1) - 2*n - 4*n^2 - 3).
 * https://oeis.org/A367967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(3^(n + 1) - 2*n - 4*n^2 - 3))/4
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3_calc.h"

seq_t a(idx_t n) {
    seq_t term     = pow3_calc(n + 1);           /* 3^(n+1) */
    seq_t two_n    = (seq_t)2 * (seq_t)n;       /* 2*n     */
    seq_t n_sq     = (seq_t)n * (seq_t)n;       /* n^2     */
    seq_t four_n_sq= (seq_t)4 * n_sq;           /* 4*n^2   */

    term -= two_n;          /* subtract 2*n */
    term -= four_n_sq;      /* subtract 4*n^2 */
    term -= (seq_t)3;       /* subtract 3   */

    term *= (seq_t)3;       /* multiply by 3 */
    term /= (seq_t)4;       /* divide by 4   */

    return term;
}
