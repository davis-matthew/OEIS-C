/*
 * A380667  First differences of the Golay-Rudin-Shapiro sequence (A020985),
 *          divided by 2.
 * https://oeis.org/A380667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (A020985(n+1) - A020985(n)) / 2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   rudine
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rudine.h"

seq_t a(idx_t n)
{
    seq_t v_n  = rudine(n);
    seq_t v_np = rudine(n + 1);
    return (v_np - v_n) / (seq_t)2;
}
