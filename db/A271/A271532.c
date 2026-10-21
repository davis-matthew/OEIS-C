/*
 * A271532  a(n) = (-1)^n*(n + 1)*(5*n^2 + 10*n + 1).
 * https://oeis.org/A271532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*(n + 1)*(5*n^2 + 10*n + 1)
 * Range:     i8 n <= 2, i16 n <= 17, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid sign/overflow issues on idx_t */
    seq_t nn = (seq_t)n;
    /* Determine the sign: (-1)^n */
    seq_t sign = ((n & 1UL) != 0) ? -1 : 1;
    /* (n + 1) */
    seq_t part = nn + 1;
    /* 5*n^2 + 10*n + 1 */
    seq_t poly = 5 * nn * nn + 10 * nn + 1;
    /* Compute the product and apply the sign */
    seq_t res = part * poly;
    res = sign * res;
    return res;
}
