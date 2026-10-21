/*
 * A291513  The arithmetic function uhat(n,2,8).
 * https://oeis.org/A291513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -47 if n is odd (1 ≤ n ≤ 9), a(n) = -48 if n is even (1 ≤
 *            n ≤ 9); for n ≥ 10: a(n) = -5 × n
 * Range:     i8 n <= 25, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 9) {
        return (seq_t)(-((n % 2) ? 47 : 48));
    }
    return (seq_t)(-5 * (seq_t)n);
}
