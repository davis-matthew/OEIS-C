/*
 * A303986  Triangle of derivatives of the Niven polynomials evaluated at 0.
 * https://oeis.org/A303986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(n,k) = (-1)^k * (n+k)!/(k!*(n-k)!) The sequence a(n) is the
 *            triangle T(n,k) flattened in row-major order: a(0)=T(0,0),
 *            a(1)=T(1,0), a(2)=T(1,1), ... This implementation computes a(n)
 *            via multiplicative binomial formulas. It works for any idx_t and
 *            seq_t defined by the build environment
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 52, i64 n <= 132,
 *            i128 n <= 347, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute binomial coefficient C(n,k) with exact integer division */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);  /* multiply by term */
        res = res / (seq_t)i;           /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index k within the row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Compute the product part: (n - k + 1) ... (n) */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        prod = prod * (seq_t)(r - k + i);
    }

    /* Compute binomial(r + k, k) */
    seq_t bin = binomial(r + k, k);

    /* Final ratio */
    seq_t res = prod * bin;

    /* Apply sign (-1)^k */
    if (k & 1) res = -res;

    return res;
}
