/*
 * A360536  Analog of Rudin-Shapiro sum sequence A020990, based on counting
 *          patterns 00 instead of 11.
 * https://oeis.org/A360536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0..n} (-1)^(i + e(i)), where e(i) counts * the
 *            (possibly overlapping) occurrences of the pattern "00" * in the
 *            binary representation of i. e(0) = 0
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   count00
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count00.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        int e = count00(i);
        idx_t parity = (i + (idx_t)e) & 1ULL;   /* 0: even, 1: odd */
        sum += (parity == 0) ? (seq_t)1 : (seq_t)(-1);
    }
    return sum;
}
