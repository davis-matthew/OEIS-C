/*
 * A255442  a(n) = A255304(2^n-1).
 * https://oeis.org/A255442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-9*a(n-3) with a(0)=1, a(1)=5, a(2)=15
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)15;
    seq_t a0 = (seq_t)1;  // a(0)
    seq_t a1 = (seq_t)5;  // a(1)
    seq_t a2 = (seq_t)15; // a(2)
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = 3 * a2 + 3 * a1 - 9 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
