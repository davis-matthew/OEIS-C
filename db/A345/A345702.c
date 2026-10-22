/*
 * A345702  Numbers that can be written as 2*a^2 - 1 and 3*b^3 - 1.
 * https://oeis.org/A345702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 648*(n-1)^6 - 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 13, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n-1)^6 in a stepwise manner to avoid unnecessary intermediate growth. */
    seq_t x   = (seq_t)(n - 1);        /* n-1  */
    seq_t x2  = x * x;                 /* (n-1)^2 */
    seq_t x4  = x2 * x2;               /* (n-1)^4 */
    seq_t x6  = x4 * x2;               /* (n-1)^6 */
    return 648 * x6 - 1;               /* 648*(n-1)^6 - 1 */
}
