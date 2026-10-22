/*
 * A377879  Deficiency of squares: a(n) = 2n^2 - sigma(n^2).
 * https://oeis.org/A377879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n^2 - sigma(n^2)
 * Range:     i8 n <= 12, i16 n <= 167, i32 n <= 9742, i64 n <= 9742,
 *            i128 n <= 9742, big any n
 * Helpers:   sigma_square_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_square_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1, but safeguard */

    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;
    seq_t sigma_sq = sigma_square_of(n);
    return 2 * n2 - sigma_sq;
}
