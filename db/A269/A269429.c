/*
 * A269429  Alternating sum of octagonal pyramidal numbers.
 * https://oeis.org/A269429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((2*n^3 + 4*n^2 - 1)*(-1)^n + 1)/4
 * Range:     i8 n <= 3, i16 n <= 24, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* convert unsigned index to signed seq_t */
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t n3 = n2 * ns;           /* n^3 */
    seq_t expr = (2 * n3) + (4 * n2) - 1;   /* 2*n^3 + 4*n^2 - 1 */
    seq_t sign = (n & 1) ? -1 : 1;          /* (-1)^n: 1 for even, -1 for odd */
    seq_t val = expr * sign;     /* apply the sign */
    return (val + 1) / 4;        /* divide by 4, exact */
}
