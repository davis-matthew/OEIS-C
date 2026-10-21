/*
 * A296367  Number of triangles on a 4 X n grid.
 * https://oeis.org/A296367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n-1)*(5*n+2) - 4*floor((n-1)^2/4) -
 *            4*floor((n-1)*(n-2)/6)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 754, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;

    /* term1 = 2*n*(n-1)*(5*n+2) */
    seq_t term1 = (seq_t)2 * ns;
    term1 *= (ns - 1);
    term1 *= (5 * ns + 2);

    /* term2 = 4*floor((n-1)^2/4) */
    seq_t t = (ns - 1) * (ns - 1);
    seq_t term2 = (t / 4) * 4;

    /* term3 = 4*floor((n-1)*(n-2)/6) */
    seq_t t2 = (ns - 1) * (ns - 2);
    seq_t term3 = (t2 / 6) * 4;

    return term1 - term2 - term3;
}
