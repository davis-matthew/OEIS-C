/*
 * A348845  Part two of the trisection of A017101: a(n) = 11 + 24*n.
 * https://oeis.org/A348845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 + 24*n
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    return ((seq_t)n * (seq_t)24) + (seq_t)11;
}
