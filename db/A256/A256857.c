/*
 * A256857  a(n) = n*(n^2 + 3*n - 2)/2.
 * https://oeis.org/A256857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 3*n - 2)/2
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t t  = tn * tn + 3 * tn - (seq_t)2;
    return tn * t / (seq_t)2;
}
