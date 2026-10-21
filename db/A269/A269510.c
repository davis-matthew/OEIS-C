/*
 * A269510  Duplicate of A093907.
 * https://oeis.org/A269510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*(floor(n/2)+1)^2
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n / 2) + 1;
    return 2 * k * k;
}
