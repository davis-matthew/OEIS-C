/*
 * A327705  The minimal size of a partition lambda of n such that every
 *          partition of n with at most 5 parts can be obtained by coalescing
 *          the parts of lambda.
 * https://oeis.org/A327705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(floor(4*n/5)), a(0) = 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;
    seq_t cur = n;
    while (cur > 0) {
        ++res;
        cur = (cur * 4) / 5;  /* floor division */
    }
    return res;
}
