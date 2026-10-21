/*
 * A249996  Expansion of 1/((1+2*x)*(1+3*x)*(1-4*x)).
 * https://oeis.org/A249996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 14*a(n-2) + 24*a(n-3); a(0) = 1, a(1) = -1,
 *            a(2) = 15
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)15;

    /* rolling variables: a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;      /* a(0)   */
    seq_t a1 = (seq_t)-1;     /* a(1)   */
    seq_t a2 = (seq_t)15;     /* a(2)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t curr = -a2 + (seq_t)14 * a1 + (seq_t)24 * a0;
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
