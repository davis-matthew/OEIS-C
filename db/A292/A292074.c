/*
 * A292074  Number of minimum dominating sets in the n X n rook complement
 *          graph.
 * https://oeis.org/A292074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,3)^2 + n^2*(n-1)^2 for n>3; a(1)=1, a(2)=4,
 *            a(3)=48
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 55, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)48;
    /* For n > 3 use the explicit formula. */
    seq_t n_seq = (seq_t)n;
    seq_t binom = (n_seq * (n_seq - (seq_t)1) * (n_seq - (seq_t)2)) / (seq_t)6;
    seq_t binom_sq = binom * binom;
    seq_t term1 = (seq_t)6 * binom_sq;
    seq_t n_sq = n_seq * n_seq;
    seq_t n1_sq = (n_seq - (seq_t)1) * (n_seq - (seq_t)1);
    seq_t term2 = n_sq * n1_sq;
    return term1 + term2;
}
