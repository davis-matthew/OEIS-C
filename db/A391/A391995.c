/*
 * A391995  a(n) = (4*n^3 - 18*n^2 - 4*n + 9*(1 - (-1)^n)) / 48.
 * https://oeis.org/A391995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (4*n^3 - 18*n^2 - 4*n + 9*(1 - (-1)^n)) / 48
 * Range:     i8 n <= 5, i16 n <= 21, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */

    /* (1 - (-1)^n) is 0 for even n and 2 for odd n */
    seq_t odd_term = (n & 1) ? 18 : 0;   /* 9 * (1 - (-1)^n) */

    seq_t num = 4 * n3 - 18 * n2 - 4 * sn + odd_term;
    seq_t result = num / 48;             /* division is exact */
    return result;
}
