/*
 * A388061  a(n) = Sum_{k=0..n} 5^k * binomial(n,k) * binomial(n+1,k).
 * https://oeis.org/A388061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..n} 5^k * binomial(n,k) * binomial(n+1,k)
 *
 * Implementation uses a multiplicative recurrence for the binomial coefficients
 * and powers of five.  All calculations are performed in seq_t, which
 * satisfies the usual C arithmetic for unsigned integers or the
 * arbitrary-precision integer class used by the build.
 *
 * The loop runs in O(n) time and never uses more than O(1) extra space.
 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow5   = 1;   /* 5^k */
    seq_t c1     = 1;   /* binomial(n, k)  */
    seq_t c2     = 1;   /* binomial(n+1, k) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            pow5 = pow5 * (seq_t)5;
            c1   = c1 * (seq_t)(n - k + 1) / (seq_t)k;          /* new binom(n,k) */
            c2   = c2 * (seq_t)(n - k + 2) / (seq_t)k;          /* new binom(n+1,k) */
        }
        result += pow5 * c1 * c2;
    }
    return result;
}
