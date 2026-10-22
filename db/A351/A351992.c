/*
 * A351992  Number of minimum edge covers in the n-vertex wheel graph.
 * https://oeis.org/A351992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 if n is even, and a(n) = (n-1)*(3*n+1)/4 if n is odd
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {
        return (seq_t)n - 1;
    } else {
        seq_t nn = (seq_t)n;
        return (nn - 1) * (3 * nn + 1) / 4;
    }
}
