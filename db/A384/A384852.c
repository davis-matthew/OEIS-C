/*
 * A384852  a(n) = 2*binomial(n,2) + 3*binomial(n,3) + 4*binomial(n,4).
 * https://oeis.org/A384852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n,2)+3*binomial(n,3)+4*binomial(n,4)
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 400, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* a(0) = a(1) = 0 */

    seq_t N = (seq_t)n;                  /* cast index to result type */
    seq_t t1 = N * (N - 1);              /* n*(n-1)         */
    seq_t t2 = 0;
    seq_t t3 = 0;

    if (n >= 3)
        t2 = N * (N - 1) * (N - 2) / 2;  /* n*(n-1)*(n-2)/2  */

    if (n >= 4)
        t3 = N * (N - 1) * (N - 2) * (N - 3) / 6;  /* n*(n-1)*(n-2)*(n-3)/6 */

    return t1 + t2 + t3;
}
