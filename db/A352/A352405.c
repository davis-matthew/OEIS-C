/*
 * A352405  a(n) = binomial(n,2)*(binomial(n-1,2) + 2).
 * https://oeis.org/A352405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)*(binomial(n-1,2) + 2)
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * (nn - 1) / 2;          /* binomial(n,2)   */
    seq_t t2 = (nn - 1) * (nn - 2) / 2;    /* binomial(n-1,2) */
    return t1 * (t2 + 2);
}
