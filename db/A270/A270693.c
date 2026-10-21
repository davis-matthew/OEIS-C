/*
 * A270693  Alternating sum of centered 25-gonal numbers.
 * https://oeis.org/A270693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((-1)^n*(50*n^2 + 100*n + 29) - 21)/8
 * Range:     i8 n <= 0, i16 n <= 24, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sign = (n & 1) ? -1 : 1;
    seq_t term = (seq_t)50 * (seq_t)n * (seq_t)n;
    term += (seq_t)100 * (seq_t)n;
    term += 29;
    term = sign * term - 21;
    return term / 8;
}
