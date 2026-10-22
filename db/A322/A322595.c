/*
 * A322595  a(n) = (n^3 + 9*n^2 + 14*n + 9)/3.
 * https://oeis.org/A322595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 9*n^2 + 14*n + 9)/3
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 2340, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the numerator using seq_t arithmetic only. */
    seq_t t = nn * nn * nn;               /* n^3 */
    t += (seq_t)9 * nn * nn;              /* + 9*n^2 */
    t += (seq_t)14 * nn;                  /* + 14*n */
    t += 9;                               /* + 9   */
    t /= 3;                               /* divide by 3 */
    return t;
}
