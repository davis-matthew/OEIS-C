/*
 * A294315  a(n) = 3*n^3 + n^2.
 * https://oeis.org/A294315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 + n^2
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;        /* cast index to seq_t */
    seq_t nsq = nn * nn;       /* n^2 */
    return 3 * nsq * nn + nsq; /* 3*n^3 + n^2 */
}
