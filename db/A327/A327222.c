/*
 * A327222  a(n) = [(2n+4)r] - [nr+4r] - [nr], where [ ] = floor and r =
 *          sqrt(2).
 * https://oeis.org/A327222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(2n+4)√2⌋ - ⌊(n+4)√2⌋ - ⌊n√2⌋
 * Range:     u8 n <= 0, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n4  = n_seq + (seq_t)4;
    seq_t two_n_plus4 = (seq_t)2 * n_seq + (seq_t)4;

    seq_t term1 = floor_sqrt( (seq_t)2 * n_seq * n_seq );          /* ⌊n√2⌋   */
    seq_t term2 = floor_sqrt( (seq_t)2 * n4   * n4   );          /* ⌊(n+4)√2⌋  */
    seq_t term3 = floor_sqrt( (seq_t)2 * two_n_plus4 * two_n_plus4 ); /* ⌊(2n+4)√2⌋ */

    return term3 - term2 - term1;
}
