/*
 * A241418  First differences of Arshon's sequence, cf. A099054.
 * https://oeis.org/A241418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A099054(n+1) - A099054(n) /* Arshon's sequence A099054 is
 *            defined by: A(0) = 0; For n > 0 let n = 3*k + d with d in
 *            {0,1,2}. If k is even: A(n) = (A(k) + d) mod 3 else: A(n) =
 *            (A(k) + (2 - d)) mod 3 This rule generates the values 0,1,2, and
 *            the first differences of that sequence are exactl...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _arshon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_arshon.h"

seq_t a(idx_t n)
{
    seq_t prev = _arshon(n);          /* A099054(n)   */
    seq_t next = _arshon(n + 1);       /* A099054(n+1) */
    return next - prev;                /* a(n)         */
}
