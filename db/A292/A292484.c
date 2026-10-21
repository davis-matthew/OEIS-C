/*
 * A292484  p-INVERT of the odd positive integers, where p(S) = 1 + S - S^2.
 * https://oeis.org/A292484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 7*a(n-3) - a(n-4) for n >= 5, a(0)
 *            = -1, a(1) = -1, a(2) = 4, a(3) = 9, a(4) = 5
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 25, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)5;

    seq_t a1 = (seq_t)-1; /* a(n-4) */
    seq_t a2 = (seq_t)4;  /* a(n-3) */
    seq_t a3 = (seq_t)9;  /* a(n-2) */
    seq_t a4 = (seq_t)5;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)3 * a4 - (seq_t)4 * a3 + (seq_t)7 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
