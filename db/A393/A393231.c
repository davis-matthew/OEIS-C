/*
 * A393231  Expansion of x^2 * (3+x-x^2) / (1-x)^4.
 * https://oeis.org/A393231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n^2 + 2*n - 2)/2
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;
    seq_t temp = nn * nn + 2 * nn - 2;
    return (n1 * temp) / 2;
}
