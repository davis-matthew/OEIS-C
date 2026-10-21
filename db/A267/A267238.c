/*
 * A267238  Sum of the triangular numbers whose indices are the digits of n.
 * https://oeis.org/A267238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{digits d of n} d * (d + 1) / 2
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 10;
        seq_t tri = (seq_t)d * ((seq_t)d + 1) / 2;
        res += tri;
        m /= 10;
    }
    return res;
}
