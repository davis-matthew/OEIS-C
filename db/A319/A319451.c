/*
 * A319451  Numbers that are congruent to {0, 3, 6} mod 12; a(n) =
 *          3*floor(4*n/3).
 * https://oeis.org/A319451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n + floor(n/3))
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t q  = sn + (sn / 3);
    return q * 3;
}
