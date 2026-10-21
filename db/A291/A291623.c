/*
 * A291623  Number of maximal irredundant and minimal dominating sets in the n
 *          X n rook complement graph.
 * https://oeis.org/A291623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 6*binomial(n,3)^2 + n^2*(n-1)^2 +
 *            12*binomial(n,4)*binomial(n,2) for n > 2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 47, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;

    seq_t ns = (seq_t)n;
    seq_t res = 0;

    /* 2*n */
    res += (seq_t)2 * ns;

    /* 6 * binom(n,3)^2 */
    seq_t binom3 = ns * (ns - 1) * (ns - 2) / (seq_t)6;
    res += (seq_t)6 * binom3 * binom3;

    /* n^2 * (n-1)^2 */
    res += ns * ns * (ns - 1) * (ns - 1);

    /* 12 * binom(n,4) * binom(n,2) */
    seq_t binom4 = ns * (ns - 1) * (ns - 2) * (ns - 3) / (seq_t)24;
    seq_t binom2 = ns * (ns - 1) / (seq_t)2;
    res += (seq_t)12 * binom4 * binom2;

    return res;
}
