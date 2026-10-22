/*
 * A368205  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3), with a(0)=1, a(1)=3 and
 *          a(2)=7.
 * https://oeis.org/A368205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) with a(0)=1, a(1)=3, a(2)=7
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 38, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)7;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        seq_t an = 3 * a2 - 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
