/*
 * A291499  The arithmetic function uhat(n,1,5).
 * https://oeis.org/A291499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -19 for n <= 6, a(n) = -3*n for n >= 7
 * Range:     i8 n <= 42, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 6) {
        return (seq_t)-19;
    }
    return ((seq_t)-3) * (seq_t)n;
}
