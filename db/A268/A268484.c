/*
 * A268484  a(n) = (n + 1)*(4*n^2 + 14*n + 9)/3.
 * https://oeis.org/A268484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(4*n^2 + 14*n + 9)/3
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns  = (seq_t)n;          /* n as seq_t */
    seq_t n1  = ns + (seq_t)1;     /* n + 1 */
    seq_t term = 4 * ns * ns       /* 4*n^2 */
                + 14 * ns           /* +14*n */
                + (seq_t)9;         /* +9 */
    /* (n+1)*(4*n^2 + 14*n + 9) is always divisible by 3 */
    return (n1 * term) / (seq_t)3;
}
