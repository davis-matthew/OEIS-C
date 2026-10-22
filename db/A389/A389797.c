/*
 * A389797  Centered 27-gonal numbers.
 * https://oeis.org/A389797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n^2 - 27*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)27 * nn * nn; /* 27*n^2 */
    term -= (seq_t)27 * nn;           /* -27*n   */
    term += (seq_t)2;                 /* +2      */
    term /= (seq_t)2;                 /* divide by 2 */
    return term;
}
