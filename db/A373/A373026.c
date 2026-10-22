/*
 * A373026  a(n) is the least positive integer k such that 3*n^2 + 2*n - k is
 *          a square.
 * https://oeis.org/A373026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the least positive integer k such that 3*n^2 + 2*n - k
 * is a perfect square.
 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert the index to seq_t */
    seq_t n_t = (seq_t)n;
    /* Compute v = 3*n^2 + 2*n */
    seq_t v = 3 * n_t * n_t + 2 * n_t;
    /* Largest square strictly less than v */
    seq_t t = floor_sqrt(v - (seq_t)1);
    /* k = v - t^2 */
    seq_t k = v - t * t;
    return k;
}
