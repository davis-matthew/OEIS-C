/*
 * A329837  Beatty sequence for (4+sqrt(26))/5.
 * https://oeis.org/A329837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(4 + sqrt(26)) / 5)
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Handle the offset: for n == 0 we return 0, though the sequence
       starts at n = 1. This guards against accidental use with n == 0. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;
    /* Compute floor(n * sqrt(26)) as floor(sqrt(26 * n^2)) */
    seq_t sq = nn * nn;
    sq *= 26;
    seq_t s = floor_sqrt(sq);

    /* a(n) = floor((4n + floor(n*sqrt(26))) / 5) */
    return (4 * nn + s) / 5;
}
