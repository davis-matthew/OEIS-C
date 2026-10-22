/*
 * A359366  a(n) = (1/8)*(((3*n + 1) + (n - 1)*(-1)^n)*(n + 1)).
 * https://oeis.org/A359366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)/2)^2 if n is odd, else (n^2 + n)/2
 * Range:     u8 n <= 23, u16 n <= 361, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (n & 1U) {
        seq_t half = (N + 1) / 2;
        return half * half;
    } else {
        return (N * (N + 1)) / 2;
    }
}
