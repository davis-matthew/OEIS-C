/*
 * A385329  a(n) = 5^n - 2*4^(n-1)*(n+4) + 3^(n-2)*(n^2+5*n+9).
 * https://oeis.org/A385329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 2*4^(n-1)*(n+4) + 3^(n-2)*(n^2+5n+9)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 4 are zero according to the OEIS data. */
    if (n < 4)
        return (seq_t)0;

    /* Compute 5^n */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= (seq_t)5;

    /* Compute 4^(n-1) */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow4 *= (seq_t)4;

    /* Compute 3^(n-2) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow3 *= (seq_t)3;

    /* Convert n to seq_t once */
    seq_t ns = (seq_t)n;

    /* term1 = 5^n */
    seq_t term1 = pow5;

    /* term2 = 2 * 4^(n-1) * (n + 4) */
    seq_t term2 = 2 * pow4 * (ns + (seq_t)4);

    /* term3 = 3^(n-2) * (n^2 + 5n + 9) */
    seq_t ns_sq = ns * ns;
    seq_t term3 = pow3 * (ns_sq + 5 * ns + (seq_t)9);

    /* a(n) = term1 - term2 + term3 */
    return term1 - term2 + term3;
}
