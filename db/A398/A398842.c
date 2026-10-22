/*
 * A398842  Lower (-2, 3/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(n*(5*n-7)/4);
 * Range:     i8 n <= 5, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   floor_div4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_div4.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * (5 * nn - 7);   /* n*(5*n-7) */
    return floor_div4(num);
}
