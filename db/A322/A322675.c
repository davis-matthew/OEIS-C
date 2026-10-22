/*
 * A322675  a(n) = n * (4*n + 3)^2.
 * https://oeis.org/A322675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (4*n + 3)^2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 644, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t  = 4 * nn + 3;
    return nn * t * t;
}
