/*
 * A343890  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,n+1,x) (rising powers of x).
 * https://oeis.org/A343890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, k) = (-1)^k * n! * binomial(2*n+1, n-k) / k!
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the triangle read row‑wise (row r has r+1 terms).     */
#include <math.h>
seq_t a(idx_t n) {
    /* Find the row r and column k such that n = r*(r+1)/2 + k */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1) - 1) / 2);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t k = n - r * (r + 1) / 2;

    /* Compute n! / k! = product_{i = k+1}^{r} i   (empty product = 1) */
    seq_t val = 1;
    for (idx_t i = k + 1; i <= r; ++i) {
        val *= (seq_t)i;
    }

    /* Compute binomial(2r+1, r-k) multiplicatively */
    idx_t m = r - k;               /* the binomial lower part */
    idx_t N = 2 * r + 1;           /* total */
    seq_t binom = 1;
    for (idx_t i = 1; i <= m; ++i) {
        binom *= (seq_t)(N - m + i);
        binom /= (seq_t)i;         /* exact division */
    }

    val *= binom;                  /* multiply by the binomial */

    /* Apply the sign (-1)^k */
    if (k & 1) val = -val;

    return val;
}
