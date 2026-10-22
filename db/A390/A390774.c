/*
 * A390774  Numbers whose final digit is at most 3.
 * https://oeis.org/A390774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10 * floor(n/4) + (n mod 4)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t q = tn / (seq_t)4;
    seq_t r = tn % (seq_t)4;
    return q * (seq_t)10 + r;
}
