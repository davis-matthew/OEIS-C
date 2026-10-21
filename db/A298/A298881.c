/*
 * A298881  a(0) = 0; for n>0, a(n) = 6*n!.
 * https://oeis.org/A298881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)i;
    }
    return res * 6;
}
