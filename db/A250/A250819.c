/*
 * A250819  Number of (7+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1695/4)*n^4 + 3786*n^3 + (54287/4)*n^2 + (33539/2)*n +
 *            6561
 * Range:     u8 none, u16 n <= 1, u32 n <= 54, u64 n <= 126, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator 4*a(n) using Horner's method to avoid intermediate overflow.
       The coefficients for the expanded polynomial of 4*a(n) are:
       1695*n^4 + 15144*n^3 + 54287*n^2 + 67078*n + 26244.
       After the Horner evaluation we divide by 4 to obtain a(n). */
    __int128 num = 1695;                 /* start with coefficient of n^4 */
    __int128 nn  = ( __int128 ) n;       /* promote n for 128‑bit arithmetic */

    num = num * nn + 15144;              /* (1695*n + 15144) * n + 54287 ... */
    num = num * nn + 54287;
    num = num * nn + 67078;
    num = num * nn + 26244;

    num = num / 4;                       /* final division to get a(n) */

    return (seq_t)num;
}
