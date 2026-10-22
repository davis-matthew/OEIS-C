/*
 * A390342  Upper (1/2)-midsequence of binomial(n,3) and binomial(n,2); see
 *          Comments.
 * https://oeis.org/A390342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((n+1)*n*(n-1)/12)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the expression is zero */
    if (n < 2)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t prod = n_seq * (n_seq + 1) * (n_seq - 1);
    return (prod + 11) / 12;
}
