/*
 * A257132  a(n) = cpg(n, 3) * cpg(n, 4) * ... * cpg(n, n) where cpg(n, m) is
 *          the m-th n-th-order centered polygonal number.
 * https://oeis.org/A257132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{m=3}^{n} (n*m*(m-1)/2 + 1) (with a(0)=a(1)=a(2)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3)
        return 1;

    seq_t prod = 1;
    seq_t N = (seq_t)n;
    for (idx_t m = 3; m <= n; ++m) {
        seq_t M = (seq_t)m;
        seq_t term = N * M * (M - 1) / 2 + 1;
        prod *= term;
    }
    return prod;
}
