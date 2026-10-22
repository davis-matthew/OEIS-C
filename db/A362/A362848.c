/*
 * A362848  a(n) = Sum_{k=0..n} 4^k * Gamma(n + k + 1/2) / Gamma(n - k + 1/2).
 *          Row sums of A362847.
 * https://oeis.org/A362848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n} (2*(n + k) - 1)!! / (2*(n - k) - 1)!!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum   = (seq_t)1;   /* term for k = 0 */
    seq_t term  = (seq_t)1;   /* current term r(n,k) */
    seq_t nn    = (seq_t)n;

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t kk     = (seq_t)k;
        seq_t factor1 = (seq_t)2 * nn - (seq_t)2 * kk + (seq_t)1;
        seq_t factor2 = (seq_t)2 * nn + (seq_t)2 * kk - (seq_t)1;
        term = term * factor1 * factor2;
        sum += term;
    }

    return sum;
}
