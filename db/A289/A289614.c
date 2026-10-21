/*
 * A289614  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 283) or the same sequence
 *          for the mesh pattern (12, 433).
 * https://oeis.org/A289614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 1 for n = 0 * a(n) = 1 for n = 1 * a(n) = 1 for n = 2 *
 *            a(n) = 2 for n = 3 * a(n) = 9 for n = 4 * a(n) = Catalan(n) -
 *            2*n + 2 for n > 4 * * The sequence is defined by the catalan
 *            numbers minus 2*n plus 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)9;
    /* for n > 4 */
    seq_t cat = catalan(n);
    seq_t two_n = (seq_t)n * 2;
    return cat - two_n + (seq_t)2;
}
