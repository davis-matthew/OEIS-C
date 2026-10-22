/*
 * A323202  Expansion of (1 - x) * (1 - x^3) / (1 - x^4) in powers of x.
 * https://oeis.org/A323202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n + [n even] * (-1)^(n/2) (with a(0)=1)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */
    if (n & (idx_t)1) return (seq_t)-1;    /* odd n: a(n) = -1 */
    /* even n: a(n) = (-1)^(n/2) + 1 */
    if ((n >> 1) & (idx_t)1) return (seq_t)0; /* n/2 odd -> a(n)=0 */
    return (seq_t)2;                       /* n/2 even -> a(n)=2 */
}
