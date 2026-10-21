/*
 * A289783  p-INVERT of the (3^n), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)5; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = 7 * a1 - 11 * a0;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
