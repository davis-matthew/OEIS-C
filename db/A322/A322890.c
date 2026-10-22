/*
 * A322890  a(n) = value of Chebyshev T-polynomial T_n(20).
 * https://oeis.org/A322890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 20, a(n) = 40*a(n-1) - a(n-2) for n > 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 20;
    seq_t prev2 = 1;
    seq_t prev1 = 20;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 40 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
