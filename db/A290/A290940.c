/*
 * A290940  Number of 6-cycles in the n-triangular graph.
 * https://oeis.org/A290940
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n,4)*(n^3 + 27*n^2 - 220*n + 392)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t nn = (seq_t)n;
    /* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    seq_t binom = nn * (nn - 1) * (nn - 2) * (nn - 3) / (seq_t)24;
    /* polynomial part */
    seq_t poly = nn * nn * nn
                 + (seq_t)27 * nn * nn
                 - (seq_t)220 * nn
                 + (seq_t)392;
    return (seq_t)2 * binom * poly;
}
