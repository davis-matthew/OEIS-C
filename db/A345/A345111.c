/*
 * A345111  a(n) = n + A345110(n).
 * https://oeis.org/A345111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + reverse_of_n_without_zeros
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        idx_t d = tmp % 10;
        if (d != 0) {
            rev = rev * 10 + (seq_t)d;
        }
        tmp /= 10;
    }
    return (seq_t)n + rev;
}
