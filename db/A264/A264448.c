/*
 * A264448  a(n) = n*(n + 11)*(n + 22)*(n + 33)/24.
 * https://oeis.org/A264448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+11)*(n+22)*(n+33)/24
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 11) * (nn + 22) * (nn + 33);
    res /= 24;
    return res;
}
