/*
 * A269693  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo n+1.
 * https://oeis.org/A269693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 21*n^5 + 15*n^4 - 13*n^3 - 11*n^2 + 3*n + 1
 *            for n > 1; * a(1) = 66
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 66;
    /* Compute the polynomial using Horner's rule */
    seq_t t = n * (n + 7) + 21;   /* t = n^2 + 7*n + 21 */
    t = t * n + 15;               /* t = n^3 + 7*n^2 + 21*n + 15 */
    t = t * n - 13;               /* t = n^4 + 7*n^3 + 21*n^2 + 15*n - 13 */
    t = t * n - 11;               /* t = n^5 + 7*n^4 + 21*n^3 + 15*n^2 - 13*n - 11 */
    t = t * n + 3;                /* t = n^6 + 7*n^5 + 21*n^4 + 15*n^3 - 13*n^2 - 11*n + 3 */
    t = t * n + 1;                /* t = n^7 + 7*n^6 + 21*n^5 + 15*n^4 - 13*n^3 - 11*n^2 + 3*n + 1 */
    return t;
}
