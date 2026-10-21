/*
 * A261540  a(n) = n^7 + 7*n^5 + 14*n^3 + 7*n.
 * https://oeis.org/A261540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^7 + 7*n^5 + 14*n^3 + 7*n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t s = (seq_t)n;          /* n as seq_t */
    seq_t n2 = s * s;                  /* n^2 */
    seq_t n3 = n2 * s;                 /* n^3 */
    seq_t n4 = n2 * n2;                /* n^4 */
    seq_t n5 = n2 * n3;                /* n^5 */
    seq_t n7 = n3 * n4;                /* n^7 */

    seq_t term1 = 7 * s;               /* 7*n */
    seq_t term3 = 14 * n3;             /* 14*n^3 */
    seq_t term5 = 7 * n5;              /* 7*n^5 */
    seq_t term7 = n7;                  /* n^7 */

    return term1 + term3 + term5 + term7;
}
