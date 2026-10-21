/*
 * A252839  Number of ways of n-coloring the square grid graph G_(5,5) such
 *          that no rectangle exists with sides parallel to the axes having
 *          all 4 corners of the same color.
 * https://oeis.org/A252839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)* (n^22 + 3*n^21 + 7*n^20 - 85*n^19 -
 *            269*n^18 - 237*n^17 + 4177*n^16 + 8805*n^15 - 12139*n^14 -
 *            109047*n^13 - 9663*n^12 + 503305*n^11 + 477366*n^10 -
 *            1657912*n^9 - 986868*n^8 - 206320*n^7 + 7988176*n^6 -
 *            5442232*n^5 - 2404368*n^4 - 428800*n^3 + 3512736*n^2 - 1855392*n
 *            + 585552)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 6, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u) /* a(0), a(1), a(2) are zero */
        return 0;

    seq_t N = (seq_t)n;

    /* Coefficients of the 22nd degree polynomial, constant term first. */
    static const long long coeffs[23] = {
        585552LL,           /* n^0  */
        -1855392LL,         /* n^1  */
        3512736LL,          /* n^2  */
        -428800LL,          /* n^3  */
        -2404368LL,         /* n^4  */
        -5442232LL,         /* n^5  */
        7988176LL,          /* n^6  */
        -206320LL,          /* n^7  */
        -986868LL,          /* n^8  */
        -1657912LL,         /* n^9  */
        477366LL,           /* n^10 */
        503305LL,           /* n^11 */
        -9663LL,            /* n^12 */
        -109047LL,          /* n^13 */
        -12139LL,           /* n^14 */
        8805LL,             /* n^15 */
        4177LL,             /* n^16 */
        -237LL,             /* n^17 */
        -269LL,             /* n^18 */
        -85LL,              /* n^19 */
        7LL,                /* n^20 */
        3LL,                /* n^21 */
        1LL                 /* n^22 */
    };

    /* Evaluate polynomial by Horner's method */
    seq_t poly = (seq_t)coeffs[22];
    for (int i = 21; i >= 0; --i)
    {
        poly = poly * N + (seq_t)coeffs[i];
    }

    /* Compute n*(n-1)*(n-2)*poly */
    seq_t res = N * (N - 1);
    res *= (N - 2);
    res *= poly;

    return res;
}
