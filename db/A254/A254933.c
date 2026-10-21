/*
 * A254933  Triangle used for the integral of even powers of the sine and
 *          cosine functions.
 * https://oeis.org/A254933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = (-1)^k * r! / k * binomial(2r,
 *            r-k), and r, k are obtained by the row‑major enumeration of the
 *            triangle: n lies in row r if (r-1)*r/2 + 1 ≤ n ≤ r*(r+1)/2, and
 *            k = n - ((r-1)*r/2 + 1) + 1
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 36, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>             /* for standard integer types */
#include "../utils/factorial.h" /* provides seq_t factorial(idx_t n)      */
#include "../utils/binomial.h"  /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* find the row r that contains position n (row‑major ordering) */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* starting index of row r (1‑based) */
    idx_t start = ((r - 1) * r) / 2 + 1;

    /* column k within the row */
    idx_t k = n - start + 1;          /* 1 ≤ k ≤ r */

    /* compute the triangle entry */
    seq_t fact   = factorial(r);                /* r!            */
    seq_t bin    = binomial(2 * r, r - k);      /* binomial(2r, r-k) */
    seq_t term   = (fact / (seq_t)k) * bin;      /* (r! / k) * catal. */

    if ((k & 1) == 1)                        /* adjust sign */
        term = -term;

    return term;
}
