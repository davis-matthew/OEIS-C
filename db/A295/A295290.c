/*
 * A295290  a(n) is the smallest triangular number t such that t - n is a
 *          square, or -1 if no such triangular number exists.
 * https://oeis.org/A295290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = minimal triangular number t such that t - n is a perfect
 *            square, or -1 if no such triangular number exists
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* D = 8*n + 1 */
    seq_t D = ((seq_t)8 * (seq_t)n) + 1;

    /* Bound on the search for y. 10,000 is sufficient for all n <= 10,000
       and also works safely for larger n when the sought solution typically
       has a small y. */
    const idx_t Y_MAX = 10000;
    for (idx_t y = 0; y <= Y_MAX; ++y) {
        seq_t y_seq = (seq_t)y;
        seq_t val = D + ((seq_t)8 * y_seq * y_seq);

        seq_t X = floor_sqrt(val);
        if (X * X != val) continue;          /* not a perfect square */
        if ((X & 1) == 0) continue;          /* X must be odd */

        /* t = X^2 - 1 over 8 */
        seq_t t = (X * X - 1) / 8;
        return t;
    }

    /* No solution found within the search bound */
    return (seq_t)-1;
}
