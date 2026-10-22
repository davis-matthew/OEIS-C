/*
 * A326725  a(n) = (1/2)*n*(5*n - 7); row 5 of A326728.
 * https://oeis.org/A326725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/2)*n*(5*n - 7)
 * Range:     i8 n <= 5, i16 n <= 114, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid truncation */
    seq_t nn = (seq_t)n;
    seq_t mul5 = nn * 5;          /* 5*n  */
    seq_t inner = mul5 - 7;       /* 5*n - 7 */
    seq_t prod = nn * inner;      /* n*(5*n - 7) */
    return prod / 2;              /* divide by 2 (always even) */
}
