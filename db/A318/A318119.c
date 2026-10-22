/*
 * A318119  Constant term in the expansion of (Sum_{k=0..n} k*(x^k +
 *          x^(-k)))^5.
 * https://oeis.org/A318119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n * (n+1) * (2*n+1) * (967*n^5 + 3868*n^4 +
 *            6005*n^3 + 5444*n^2 + 2844*n + 1008) / 9072
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t N     = (seq_t)n;          /* cast index to seq_t */
    seq_t n1    = N - 1;
    seq_t n2    = N + 1;
    seq_t two_n1 = (seq_t)2 * N + 1; /* 2*n + 1 */

    /* powers of n */
    seq_t n2t = N * N;               /* n^2 */
    seq_t n3  = n2t * N;             /* n^3 */
    seq_t n4  = n3 * N;              /* n^4 */
    seq_t n5  = n4 * N;              /* n^5 */

    /* polynomial part */
    seq_t poly = (seq_t)967 * n5;
    poly += (seq_t)3868 * n4;
    poly += (seq_t)6005 * n3;
    poly += (seq_t)5444 * n2t;
    poly += (seq_t)2844 * N;
    poly += (seq_t)1008;             /* constant term */

    /* full product */
    seq_t prod = n1 * N;
    prod = prod * n2;
    prod = prod * two_n1;
    prod = prod * poly;

    return prod / (seq_t)9072;
}
