/*
 * A356205  T(n,k) are the numerators of the coefficients of the Legendre
 *          polynomials of degree n, with increasing exponents, where T(n,k)
 *          is a triangle read by rows.
 * https://oeis.org/A356205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, frac
 * Formula:   a(n) = the numerator of the coefficient of x^c in the Legendre *
 *            polynomial P_r(x), where r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *            * and c = n - r(r+1)/2. * If r and c have different parity the
 *            coefficient is 0. * Otherwise let k = (r - c)/2. The numerator
 *            before reduction equals * binom(r,k) * bi...
 * Range:     i8 n <= 13, i16 n <= 43, i32 n <= 180, i64 n <= 551,
 *            i128 n <= 1935, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Compute the position within the row */
    idx_t terms_before = r * (r + 1) / 2;
    idx_t c = n - terms_before;  /* exponent (column) */

    /* Parity check: if r and c differ, the coefficient is 0 */
    if (((r - c) & 1) != 0) {
        return (seq_t)0;
    }

    idx_t k = (r - c) / 2;               /* k = (r - c)/2 */

    /* Binomial coefficients needed for the numerator */
    seq_t b1 = binom(r, k);              /* C(r, k) */
    seq_t b2 = binom(r + c, r);          /* C(r + c, r) = C(2r-2k, r) */

    seq_t raw_abs = b1 * b2;              /* product is positive */

    /* Remove all factors of 2 by shifting right until odd */
    while ((raw_abs & 1) == 0) {
        raw_abs >>= 1;
    }

    /* Apply sign (-1)^k */
    if ((k & 1) != 0) {
        return -(raw_abs);
    }
    return raw_abs;
}
