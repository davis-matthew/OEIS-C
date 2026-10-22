/*
 * A317633  Numbers congruent to {1, 7, 9} mod 10.
 * https://oeis.org/A317633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 4*floor((n+1)/3) - 1
 * Range:     u8 n <= 76, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t d = (n + 1) / 3;
    seq_t div = (seq_t)d;
    seq_t n_s = (seq_t)n;
    return 2 * n_s + 4 * div - 1;
}
