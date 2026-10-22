/*
 * A347922  Number of minimal total dominating sets in the n X n rook
 *          complement graph.
 * https://oeis.org/A347922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,3)^2 + 2*binomial(n,2)^3 - binomial(n,2)^2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)  /* a(0) and a(1) are 0 */
        return 0;

    seq_t nn = (seq_t)n;

    /* binomial(n,2) = n*(n-1)/2 */
    seq_t binom2 = nn * (nn - 1) / 2;

    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    seq_t binom3 = nn * (nn - 1) * (nn - 2) / 6;

    /* 6 * binom3^2 */
    seq_t term1 = binom3 * binom3;
    term1 *= 6;

    /* 2 * binom2^3 */
    seq_t term2 = binom2 * binom2;
    term2 *= binom2;
    term2 *= 2;

    /* binom2^2 */
    seq_t term3 = binom2 * binom2;

    return term1 + term2 - term3;
}
