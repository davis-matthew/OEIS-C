/*
 * A339032  Expansion of (4*x^5 - 9*x^4 + 17*x^3 - 15*x^2 + 6*x - 1)/((2*x -
 *          1)^2*(x - 1)^3).
 * https://oeis.org/A339032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 2)*2^n - 2*n^2 - 2)/2 (n >= 1); a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* use the equivalent form to avoid overflow:
       a(n) = (n + 2)*2^(n-1) - (n^2 + 1)          */
    seq_t pow_two = pow2(n - 1);          /* 2^(n-1)   */
    seq_t term     = (seq_t)(n + 2) * pow_two;

    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;          /* n^2 */
    seq_t sub  = n2 + 1;                 /* n^2 + 1 */

    return term - sub;
}
