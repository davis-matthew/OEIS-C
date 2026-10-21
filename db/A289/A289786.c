/*
 * A289786  p-INVERT of the odd positive integers (A005408), where p(S) = 1 -
 *          S - S^2.
 * https://oeis.org/A289786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 5*a(n-3) + a(n-4) * Initial values:
 *            a(0)=1, a(1)=5, a(2)=20, a(3)=77
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)77;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)5;
    seq_t a2 = (seq_t)20;
    seq_t a3 = (seq_t)77;
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)5 * a3;
        next -= (seq_t)6 * a2;
        next += (seq_t)5 * a1;
        next += a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
