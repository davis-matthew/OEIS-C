/*
 * A325173  Perfect squares of the form a + b^2 + c^3, where a,b,c are
 *          consecutive numbers.
 * https://oeis.org/A325173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n^2 + 2)^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t t  = n2 + (seq_t)2;    /* n^2 + 2 */
    t = t * t;                   /* (n^2 + 2)^2 */
    return n2 * t;                /* n^2 * (n^2 + 2)^2 */
}
