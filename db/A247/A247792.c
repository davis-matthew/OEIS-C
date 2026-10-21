/*
 * A247792  a(n) = 9*n^2 + 1.
 * https://oeis.org/A247792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 + 1
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 9 * n * n + 1 in the seq_t type.  All intermediate
       values are cast to seq_t before multiplication to maintain
       correct arithmetic in whatever underlying type seq_t may be. */
    return (seq_t)9 * (seq_t)n * (seq_t)n + 1;
}
