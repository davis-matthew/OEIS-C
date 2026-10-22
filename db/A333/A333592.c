/*
 * A333592  a(n) = Sum_{k = 0..n} binomial(n+k-1, k)^2.
 * https://oeis.org/A333592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n+k-1, k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 1;          /* k = 0 term: C(n-1,0)^2 = 1 */
    seq_t c   = 1;          /* current binomial C(n+k-1, k)   */

    for (idx_t k = 0; k < n; ++k) {
        /* Compute next binomial C(n+k, k+1) from current c */
        c = c * ((seq_t)n + (seq_t)k);
        c = c / ((seq_t)k + 1);

        seq_t sq = c * c;   /* square the binomial */
        sum += sq;          /* accumulate the sum */
    }
    return sum;
}
