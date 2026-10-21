/*
 * A297675  a(n) = 3*(n^2+n-4)/2.
 * https://oeis.org/A297675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n^2+n-4)/2
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to the sequence type      */
    seq_t v  = nn * nn + nn;        /* n^2 + n                             */
    v -= (seq_t)4;                  /* subtract 4                          */
    v = v * (seq_t)3 / (seq_t)2;    /* multiply by 3, then divide by 2     */
    return v;
}
