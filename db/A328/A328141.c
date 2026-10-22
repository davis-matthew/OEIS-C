/*
 * A328141  a(n) = a(n-1) - (n-2)*a(n-2), with a(0)=1, a(1)=2.
 * https://oeis.org/A328141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - (n-2)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 21, i64 n <= 34, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a0 = 1, a1 = 2, an;
    for (idx_t k = 2; k <= n; ++k) {
        an = a1 - ((seq_t)(k - 2) * a0);
        a0 = a1;
        a1 = an;
    }
    return a1;
}
