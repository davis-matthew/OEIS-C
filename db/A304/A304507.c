/*
 * A304507  a(n) = 5*(n+1)*(9*n+4).
 * https://oeis.org/A304507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)*(9*n+4)
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 5 * (nn + 1) * (9 * nn + 4);
}
