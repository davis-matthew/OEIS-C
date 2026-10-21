/*
 * A266464  Number of n X 2 binary arrays with rows and columns
 *          lexicographically nondecreasing and column sums nonincreasing.
 * https://oeis.org/A266464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 3*n^2 + 22*n + (n%2?21:24))/24
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1393, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = 2 * nn * nn * nn + 3 * nn * nn + 22 * nn
                + ( (n & 1ULL) ? (seq_t)21 : (seq_t)24 );
    return num / 24;
}
