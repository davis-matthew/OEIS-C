/*
 * A390565  a(n) = floor(n^2/3 + n^3/2).
 * https://oeis.org/A390565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/3 + n^3/2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 and n^3 in seq_t arithmetic */
    seq_t nn   = (seq_t)n * (seq_t)n;       /* n^2 */
    seq_t nnn  = nn * (seq_t)n;             /* n^3 */

    /* Floor divisions: natural integer truncation is floor for positives */
    seq_t t1 = nn / (seq_t)3;               /* floor(n^2 / 3) */
    seq_t t2 = nnn / (seq_t)2;              /* floor(n^3 / 2) */

    return t1 + t2;                         /* a(n) */
}
