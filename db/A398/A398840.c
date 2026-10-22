/*
 * A398840  Lower (1, 3/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(11*n-1)/4)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* may be needed for types, included just in case */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* compute n*(11*n-1) as unsigned, then divide by 4 */
    return (nn * (((seq_t)11 * nn) - 1)) / 4;
}
