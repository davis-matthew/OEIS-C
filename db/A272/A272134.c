/*
 * A272134  a(n) = n*(15*n^2 - 15*n + 4).
 * https://oeis.org/A272134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n^2 - 15*n + 4)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 659, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* cast index to seq_t first to avoid implicit integer promotion issues */
    seq_t nn = (seq_t)n;
    /* compute n^2 to avoid repeating multiplication */
    seq_t n2 = nn * nn;
    /* compute the expression 15*n^2 - 15*n + 4 */
    seq_t inner = (seq_t)15 * n2 - (seq_t)15 * nn + (seq_t)4;
    /* finally multiply by n */
    return nn * inner;
}
