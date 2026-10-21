/*
 * A252780  Number of ways of n-coloring the square grid graph G_(4,4) such
 *          that no rectangle exists with sides parallel to the axes having
 *          all 4 corners of the same color.
 * https://oeis.org/A252780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^14 + n^13 + n^12 -35*n^11 -35*n^10 +61*n^9 *
 *            +547*n^8 -101*n^7 -1797*n^6 -633*n^5 * +5655*n^4 -5625*n^3
 *            +3684*n^2 -2436*n +852)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 256,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t ns = (seq_t)n;
    seq_t poly = 0;

    /* Horner’s rule from the highest degree down to the constant term */
    poly = poly * ns + (seq_t)1;      /* coeff of n^14  */
    poly = poly * ns + (seq_t)1;      /* coeff of n^13  */
    poly = poly * ns + (seq_t)1;      /* coeff of n^12  */
    poly = poly * ns + (seq_t)-35;    /* coeff of n^11  */
    poly = poly * ns + (seq_t)-35;    /* coeff of n^10  */
    poly = poly * ns + (seq_t)61;     /* coeff of n^9   */
    poly = poly * ns + (seq_t)547;    /* coeff of n^8   */
    poly = poly * ns + (seq_t)-101;   /* coeff of n^7   */
    poly = poly * ns + (seq_t)-1797;  /* coeff of n^6   */
    poly = poly * ns + (seq_t)-633;   /* coeff of n^5   */
    poly = poly * ns + (seq_t)5655;   /* coeff of n^4   */
    poly = poly * ns + (seq_t)-5625;  /* coeff of n^3   */
    poly = poly * ns + (seq_t)3684;   /* coeff of n^2   */
    poly = poly * ns + (seq_t)-2436;  /* coeff of n^1   */
    poly = poly * ns + (seq_t)852;    /* coeff of n^0   */

    /* final multiplication with n*(n-1) */
    seq_t result = ns * (ns - 1);
    result *= poly;

    return result;
}
