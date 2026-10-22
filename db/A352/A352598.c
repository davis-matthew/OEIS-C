/*
 * A352598  a(n) is the product of the squares of the nonzero digits of n.
 * https://oeis.org/A352598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of squares of nonzero digits of n
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t res = 1;
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 10;
        if (d != 0) {
            seq_t sq = (seq_t)d * (seq_t)d;
            res *= sq;
        }
        m /= 10;
    }
    return res;
}
