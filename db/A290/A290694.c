/*
 * A290694  Triangle read by rows, numerators of coefficients (in rising
 *          powers) of rational polynomials P(n, x) such that
 *          Integral_{x=0..1} P'(n, x) = Bernoulli(n, 1).
 * https://oeis.org/A290694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, frac
 * Formula:   a(n) = sequence of numerators T(row,k) defined by T(n,k) =
 *            Numerator(Stirling2(n, k-1)*(k-1)!/k) with sign factor sign =
 *            (-1)^(k-1 + (n & 1)). Indexing follows the OEIS triangle read by
 *            rows; a(0) = first element (0)
 * Range:     i8 n <= 23, i16 n <= 48, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies required by the build system */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row (n_row) and column (k) corresponding to index n */
    idx_t idx = n;
    idx_t row = 0;
    while (1) {
        unsigned __int128 next = (unsigned __int128)(row + 1) * (row + 4) / 2; /* offset(row+1) */
        if (next > idx) break;
        row++;
    }
    unsigned __int128 off = (unsigned __int128)row * (row + 3) / 2;           /* offset(row) */
    idx_t k = idx - (idx_t)off;
    if (k == 0) return (seq_t)0;                     /* first element of each row is always 0 */

    idx_t r = k - 1;                                 /* r = k-1 >= 0 */
    int signParity = ((k - 1 + (row & 1)) & 1);      /* parity of the sign exponent */
    seq_t sign = signParity ? (seq_t)-1 : (seq_t)1;   /* sign = +/-1 */

    /* Compute G(n,r) = Σ_{j=0}^r (-1)^{r-j} * C(r,j) * j^n  */
    seq_t G = (seq_t)0;
    seq_t bin = (seq_t)1;                             /* C(r,0) = 1 */
    for (idx_t j = 0; j <= r; ++j) {
        seq_t powj = seq_pow((seq_t)j, row);           /* j^n */
        seq_t term = bin * powj;
        if ( ((r - j) & 1) == 1 ) G -= term;          /* (-1)^{r-j} */
        else                       G += term;

        /* Update bin to C(r, j+1) for next iteration */
        if (j < r) {
            bin = bin * (r - j) / (j + 1);             /* exact division */
        }
    }

    /* Reduce by gcd with (r+1) to obtain the numerator */
    seq_t g = gcd(G, (seq_t)(r + 1));
    seq_t res = G / g;
    res = res * sign;                                /* apply the sign factor */
    return res;
}
