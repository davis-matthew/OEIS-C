/*
 * A254371  Sum of cubes of the first n even numbers (A016743).
 * https://oeis.org/A254371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2*(n+1)^2
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    seq_t prod = nn * n1;   /* n * (n+1) */
    prod = prod * prod;     /* [n*(n+1)]^2 */
    return prod * (seq_t)2; /* 2 * [n*(n+1)]^2 */
}
