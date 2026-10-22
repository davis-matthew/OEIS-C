/*
 * A303303  Generalized 23-gonal (or icositrigonal) numbers: m*(21*m - 19)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(21*n + 38)/8 for n even
 * Range:     u8 n <= 9, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = (21*n - 17)*(n + 1)/8 for n odd */
seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {
        return sn * (sn * (seq_t)21 + (seq_t)38) / (seq_t)8;
    } else {
        return (sn * (seq_t)21 - (seq_t)17) * (sn + (seq_t)1) / (seq_t)8;
    }
}
