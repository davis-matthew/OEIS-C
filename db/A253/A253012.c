/*
 * A253012  a(n) = ceiling( (n+1) * (n+2) / 12).
 * https://oeis.org/A253012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (n+1) * (n+2) / 12 )
 * Range:     u8 n <= 14, u16 n <= 418, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)(n + 1) * (seq_t)(n + 2);
    return (x + 11) / 12;
}
