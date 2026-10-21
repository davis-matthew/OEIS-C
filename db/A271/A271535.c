/*
 * A271535  a(n) = ( n*(n + 1)*(2*n + 1)/6 )^2.
 * https://oeis.org/A271535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n*(n + 1)*(2*n + 1)/6 )^2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the sum of squares  S(n) = n(n+1)(2n+1)/6  */
    seq_t nn   = n;
    seq_t sum  = nn * (nn + 1);      /* n*(n+1) */
    sum       *= (2 * nn + 1);       /* n*(n+1)*(2n+1) */
    sum       /= 6;                  /* divide by 6 */

    /* Return the square of that sum */
    return sum * sum;
}
