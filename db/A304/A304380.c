/*
 * A304380  a(n) = 36*n^2 - 4*n (n>=1).
 * https://oeis.org/A304380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 4*n
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  For n < 1 simply return 0. */
    if (n < 1)
        return (seq_t)0;

    seq_t ns = (seq_t)n;          /* convert index to seq_t */
    seq_t sq = ns * ns;           /* n^2  */
    seq_t val = sq * (seq_t)36;   /* 36*n^2 */
    val -= ns * (seq_t)4;         /* subtract 4*n */
    return val;
}
