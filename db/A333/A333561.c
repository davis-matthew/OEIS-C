/*
 * A333561  a(n) = Sum_{j = 0..2*n} binomial(n+j-1,j)*2^j.
 * https://oeis.org/A333561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..2*n} binomial(n+j-1, j) * 2^j
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;          /* binomial(n + j - 1, j), start with j = 0 */
    seq_t pow2  = 1;          /* 2^j , start with j = 0 */

    for (idx_t j = 0; j <= 2 * n; ++j)
    {
        if (j > 0)
        {
            /* binom_j = binom_{j-1} * (n + j - 1) / j */
            seq_t num = binom * ((seq_t)n + (seq_t)j - 1);
            binom = num / (seq_t)j;
            /* multiply pow2 by 2 to get 2^j */
            pow2 = pow2 * ((seq_t)2);
        }
        sum += binom * pow2;
    }
    return sum;
}
