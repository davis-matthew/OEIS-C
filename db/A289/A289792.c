/*
 * A289792  Number of 4-cycles in the n-tetrahedral graph.
 * https://oeis.org/A289792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n - 1, 4) * (210 - 41*n + 7*n^2)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        return (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    /* binomial(n-1,4) = (n-1)*(n-2)*(n-3)*(n-4)/24 */
    seq_t bin = (nn - 1) * (nn - 2) * (nn - 3) * (nn - 4) / 24;
    seq_t p   = (seq_t)210 - (seq_t)41 * nn + (seq_t)7 * nn * nn;
    return bin * p / 2;
}
