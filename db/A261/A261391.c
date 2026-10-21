/*
 * A261391  a(n) = n^5 + 5*n^3 + 5*n.
 * https://oeis.org/A261391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^5 + 5*n^3 + 5*n (OEIS A261391)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x  = (seq_t)n;        /* the index as result type   */
    seq_t x2 = x * x;          /* n^2                        */
    seq_t x3 = x2 * x;         /* n^3                        */
    seq_t x5 = x3 * x2;        /* n^5 = n^3 * n^2            */
    return x5 + 5 * x3 + 5 * x;
}
