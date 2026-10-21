/*
 * A292058  Wiener index of the n X n rook complement graph.
 * https://oeis.org/A292058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*n^2*(n + 3)/2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety for n=0, not used in OEIS offset */
    seq_t N = (seq_t)n;
    seq_t f1 = N - 1;      /* n-1 */
    seq_t f2 = N;          /* n   (will be halved if even) */
    seq_t f3 = N;          /* n   */
    seq_t f4 = N + 3;      /* n+3 */

    /* divide one of the even factors by 2 to keep intermediate values small */
    if ((f1 & 1) == 0) {
        f1 >>= 1;
    } else if ((f4 & 1) == 0) {
        f4 >>= 1;
    } else {
        f2 >>= 1;   /* N must be even in this branch */
    }

    return f1 * f2 * f3 * f4;
}
