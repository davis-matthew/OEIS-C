/*
 * A398850  Lower (-1/2, 1/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor( (-3*T(n) + 2*P(n)) / 6 ) where T(n) = n*(n+1)/2
 *            (triangular number) P(n) = n*(3*n-1)/2 (pentagonal number)
 * Range:     i8 n <= 10, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t floor_div(seq_t num, seq_t den)
{
    /* den > 0 */
    if (num >= 0)
        return num / den;
    /* handle negative numbers correctly (floor) */
    return - ((-num + den - 1) / den);
}

seq_t a(idx_t n)
{
    /* Compute triangular number T(n) = n*(n+1)/2 */
    seq_t nn = (seq_t)n;
    seq_t T = nn * (nn + (seq_t)1) / (seq_t)2;

    /* Compute pentagonal number P(n) = n*(3*n-1)/2 */
    seq_t P = nn * ((seq_t)3 * nn - (seq_t)1) / (seq_t)2;

    /* Numerator of the rational expression: -3*T + 2*P */
    seq_t num = - (seq_t)3 * T + (seq_t)2 * P;

    /* Floor division by 6 */
    return floor_div(num, (seq_t)6);
}
