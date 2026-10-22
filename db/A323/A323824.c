/*
 * A323824  a(0) = 6; thereafter a(n) = 4*a(n-1) + 1.
 * https://oeis.org/A323824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 6; thereafter a(n) = 4*a(n-1) + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 6;
    idx_t i = n;
    while (i--) {
        res = res * 4 + 1;
    }
    return res;
}
