/*
 * A276159  Convolution of nonzero octagonal numbers (A000567) with
 *          themselves.
 * https://oeis.org/A276159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(n + 3)*(9*n^2 + 6*n + 5)/30
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t t = sn + 1;
    seq_t u = sn + 2;
    seq_t v = sn + 3;
    seq_t w = 9 * sn * sn + 6 * sn + 5;
    seq_t prod = t * u;
    prod = prod * v;
    prod = prod * w;
    return prod / 30;
}
