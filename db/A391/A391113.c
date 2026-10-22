/*
 * A391113  Triangle read by rows: T(n,k) is the area of the irregular
 *          hexagon, rounded to the nearest integer, where its vertices lie on
 *          the co-vertices and the endpoints of the two latus rectums of an
 *          ellipse, where n and k are respectively the integer major and
 *          minor axes with n >= 2 and k >= 1.
 * https://oeis.org/A391113
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = round(2*sqrt(r^2−c^2)*(c+c^2/r)) where the * sequence is
 *            the triangle read by rows, offset 2 * (a(2) = T(2,1), a(3) =
 *            T(3,1), a(4) = T(3,2), …)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* undefined for n < 2; return 0 safely */
    if (n < 2) return 0;

    /* t is the number of terms before a(n) in the enumeration */
    idx_t t = n - 2;

    /* Determine the row r such that (r‑1)*r/2 > t
     * r = floor((1 + sqrt(1+8t))/2) + 1 */
    double d = sqrt(1.0 + 8.0 * (double)t);
    idx_t r = (idx_t)(floor((1.0 + d) * 0.5) + 1.0);

    /* Number of terms before this row */
    idx_t prev = (r - 2) * (r - 1) / 2;

    /* Position inside the row (0‑based) */
    idx_t offset = t - prev;

    /* Column index (k) */
    idx_t k = offset + 1;

    /* Compute the formula using double arithmetic.
       All intermediate integers are converted to double
       before the operations. */
    double rr = (double)r;
    double kk = (double)k;
    double diff = rr * rr - kk * kk;             /* r^2 - k^2 */
    double sqrt_diff = sqrt(diff);

    double value = 2.0 * sqrt_diff * ( kk + (kk * kk) / rr );

    /* Round to nearest integer */
    long long rounded = (long long)floor(value + 0.5);

    return (seq_t)rounded;
}
