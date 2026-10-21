/*
 * A291912  Number of 6-cycles in the n X n rook complement graph.
 * https://oeis.org/A291912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-2 + n)*(-1 + n)^2*n^2*(-52 + 12*n + 76*n^2 - 63*n^3 -
 *            2*n^4 + 20*n^5 - 8*n^6 + n^7)/12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first two terms are 0, other n are handled by the formula */
    if (n < 3U) return (seq_t)0;

    /* Promote n to the integer type used for sequence values */
    seq_t x  = (seq_t)n;          /* n          */
    seq_t x2 = x * x;             /* n^2        */
    seq_t x3 = x2 * x;            /* n^3        */
    seq_t x4 = x3 * x;            /* n^4        */
    seq_t x5 = x4 * x;            /* n^5        */
    seq_t x6 = x5 * x;            /* n^6        */
    seq_t x7 = x6 * x;            /* n^7        */

    /* Positive part of the polynomial:  n^7 + 20*n^5 + 76*n^2 + 12*n   */
    seq_t poly_pos = x7;
    poly_pos = poly_pos + (seq_t)20U * x5;
    poly_pos = poly_pos + (seq_t)76U * x2;
    poly_pos = poly_pos + (seq_t)12U * x;

    /* Negative part of the polynomial:  8*n^6 + 2*n^4 + 63*n^3 + 52   */
    seq_t poly_neg = (seq_t)8U * x6;
    poly_neg = poly_neg + (seq_t)2U  * x4;
    poly_neg = poly_neg + (seq_t)63U * x3;
    poly_neg = poly_neg + (seq_t)52U;

    /* The polynomial value (positive for n ≥ 3) */
    seq_t poly = poly_pos - poly_neg;

    /* Compute the prefactor: (n-2)*(n-1)^2*n^2   */
    seq_t pref = (seq_t)(n - 2U);
    pref = pref * (x - (seq_t)1U);        /* (n-2)*(n-1)   */
    pref = pref * (x - (seq_t)1U);        /* (n-2)*(n-1)^2 */
    pref = pref * x;                     /* *n^1          */
    pref = pref * x;                     /* *n^2          */

    /* Final product and division by 12 */
    pref = pref * poly;
    pref = pref / (seq_t)12U;

    return pref;
}
