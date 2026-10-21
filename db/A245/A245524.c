/*
 * A245524  a(n) = n^2 - floor(n/2)*(-1)^n.
 * https://oeis.org/A245524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - floor(n/2)*(-1)^n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t nval = (seq_t)n;
    /* n/2 (floor division) */
    seq_t half = (seq_t)(n >> 1);
    /* n^2 */
    seq_t sq = nval * nval;
    /* parity check */
    if (n & 1) {           /* n is odd: add half */
        return sq + half;
    } else {               /* n is even: subtract half */
        return sq - half;
    }
}
