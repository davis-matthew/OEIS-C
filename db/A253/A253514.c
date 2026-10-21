/*
 * A253514  Centered heptagonal numbers (A069099) which are also centered
 *          octagonal numbers (A016754).
 * https://oeis.org/A253514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 899*a(n-1) - 899*a(n-2) + a(n-3) Initial values: a(1)=1,
 *            a(2)=841, a(3)=755161
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 841;
    if (n == 3) return 755161;
    seq_t a1 = 1, a2 = 841, a3 = 755161;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a = a3 * 899 - a2 * 899 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a;
    }
    return a3;
}
