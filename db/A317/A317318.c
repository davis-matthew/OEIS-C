/*
 * A317318  Multiples of 18 and odd numbers interleaved.
 * https://oeis.org/A317318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 18*n, a(2n+1) = 2*n+1
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_half = (seq_t)(n >> 1);
    if ((n & 1ULL) == 0) {
        return n_half * (seq_t)18;
    } else {
        return n_half * (seq_t)2 + (seq_t)1;
    }
}
