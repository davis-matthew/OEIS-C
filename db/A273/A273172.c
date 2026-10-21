/*
 * A273172  Triangle for denominators of coefficients for integrated odd
 *          powers of cos(x) in terms sin((2*m+1)*x).
 * https://oeis.org/A273172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   * a(n) = denominator( binomial(2*r+1, r-c) / ( 2^(2*r) * (2*c+1)
 *            ) ) * where r and c are the row and column indices obtained from
 *            n by * r(r+1)/2 <= n < (r+1)(r+2)/2 and c = n - r(r+1)/2. * *
 *            The sequence is the row‑major flattening of the triangle whose *
 *            elements are the denominators of the int...
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri <= n) {
        tri += r + 1;
        r++;
    }
    r--;                     /* r is now the correct row */
    tri = r * (r + 1) / 2;   /* triangular number up to previous row */
    idx_t c = n - tri;       /* column within the row */

    /* Compute the binomial coefficient binomial(2*r+1, r-c). */
    seq_t B = binomial(2 * r + 1, r - c);

    /* Denominator: 2^(2*r) * (2*c + 1). */
    seq_t denom = pow2(2 * r);
    denom *= (seq_t)(2 * c + 1);

    /* Reduce by GCD. */
    seq_t g = gcd(B, denom);
    denom /= g;

    return denom;
}
