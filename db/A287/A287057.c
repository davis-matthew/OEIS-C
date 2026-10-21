/*
 * A287057  a(n) = 2*n^2 + n - (n+1) mod 2.
 * https://oeis.org/A287057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 + n - ((n+1) mod 2)
 * Range:     u8 n <= 11, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t offset = ((seq_t)n + 1) & 1;   /* (n+1) mod 2 */
    return 2 * nn * nn + nn - offset;
}
