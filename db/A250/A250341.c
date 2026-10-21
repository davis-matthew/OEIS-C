/*
 * A250341  Number of length 5+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/105)*n^9 + (34/35)*n^8 + (25/2)*n^7 + 50*n^6 + 92*n^5
 *            * + (1384/15)*n^4 + (621/14)*n^3 - (173/21)*n^2 - (193/15)*n + 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or below the offset, the value is undefined; return 0. */
    if (n == 0) return 0;

    /* Use a 128‑bit intermediate type to avoid overflow up to n ≈ 10⁴
       (the tests run up to around 10⁴).  The numerator of the polynomial
       multiplied by 210 is an integer, so integer division yields the
       correct result. */
    __int128 nn = n;
    __int128 t = 2;                /* coefficient of n^9 */
    t = t * nn + 204;              /* + 204*n^8 */
    t = t * nn + 2625;             /* + 2625*n^7 */
    t = t * nn + 10500;            /* + 10500*n^6 */
    t = t * nn + 19320;            /* + 19320*n^5 */
    t = t * nn + 19376;            /* + 19376*n^4 */
    t = t * nn + 9315;             /* + 9315*n^3 */
    t = t * nn - 1730;             /* - 1730*n^2 */
    t = t * nn - 2702;             /* - 2702*n^1 */
    t = t * nn + 210;              /* + 210          */

    return (seq_t)(t / 210);
}
