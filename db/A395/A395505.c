/*
 * A395505  Triangle of polynomials refining A367255.
 * https://oeis.org/A395505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in (1+x*n)^(n-2)*(1+2*n*x+n*x^2),
 *            where the triangle of coefficients is concatenated row by row in
 *            natural order (starting with the term of smallest degree)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_pow.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that T_r = r*(r+1)/2 <= n < T_{r+1} */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Offset inside row r */
    idx_t offset = n - r * (r + 1) / 2;   /* 0-based index within the row */
    idx_t col = offset;                 /* row coefficients are in natural order */

    /* Rows 0 and 1 are special: all coefficients are 1 */
    if (r <= 1)
        return (seq_t)1;

    /* Compute coefficient for row n = r and column = col */
    if (col > r)  /* safety check */
        return (seq_t)0;

    seq_t nseq = (seq_t)r;   /* value of 'n' in the polynomial */
    seq_t term1 = 0, term2 = 0, term3 = 0;

    /* term1: C(r-2, col) * r^col, if col <= r-2 */
    if (col <= r - 2)
        term1 = binom_pow(r - 2, col, nseq);

    /* term2: 2 * C(r-2, col-1) * r^col, if col-1 <= r-2 */
    if (col >= 1 && col - 1 <= r - 2)
        term2 = binom_pow(r - 2, col - 1, nseq) * ((seq_t)2 * nseq);

    /* term3: C(r-2, col-2) * r^(col-1), if col-2 <= r-2 */
    if (col >= 2 && col - 2 <= r - 2)
        term3 = binom_pow(r - 2, col - 2, nseq) * nseq;

    return term1 + term2 + term3;
}
