/*
 * A293070  Sum of values of vertices at level n of the hyperbolic Pascal
 *          pyramid PP_(4,5).
 * https://oeis.org/A293070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 19*a(n-2) + 14*a(n-3) for n>=3, with initial
 *            values a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = 1, a1 = 3, a2 = 9;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t nxt = 8 * a2 - 19 * a1 + 14 * a0;
        a0 = a1;
        a1 = a2;
        a2 = nxt;
    }
    return a2;
}
