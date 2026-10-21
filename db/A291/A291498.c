/*
 * A291498  The arithmetic function uhat(n,1,4).
 * https://oeis.org/A291498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -11 for 1 ≤ n ≤ 5; a(n) = -2*n for n ≥ 6
 * Range:     i8 n <= 64, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5u) {
        return (seq_t)-11;
    } else {
        return (seq_t)(-2 * (seq_t)n);
    }
}
