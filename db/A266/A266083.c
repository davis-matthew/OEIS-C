/*
 * A266083  a(n) = Sum_{k = 0..n - 1} (a(n - 1) + k) for n>0, a(0) = 1.
 * https://oeis.org/A266083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + binomial(n,2) a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* a(0) */
    idx_t i;
    for (i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term = (ii * (ii - 1)) / 2;   /* binomial(i,2) */
        res = ii * res + term;
    }
    return res;
}
