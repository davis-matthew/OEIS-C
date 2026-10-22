/*
 * A367023  Triangle read by rows, T(n, k) = [x^k] p(n), where p(n) =
 *          hypergeom([1/2, -n - 1, -n], [2, 2], 4*x).
 * https://oeis.org/A367023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = binomial(n,k) * binomial(n+1,k) *
 *            binomial(2k,k) / (k+1)^2 with r = floor((sqrt(8*n+1)-1)/2) and k
 *            = n - r*(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute binomial coefficient C(n,k) for small k
   using a multiplicative algorithm that stays in seq_t. */
static seq_t binomial_small(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        result *= (seq_t)(n - k + i);   /* multiply by numerator part */
        result /= (seq_t)i;             /* divide by denominator part */
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);

    /* Adjust r to satisfy the bounds (use 128-bit to avoid overflow) */
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) ++r;
    while (((unsigned __int128)r * (r + 1)) / 2 > n) --r;

    /* Local index k in the row */
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - start;

    /* Compute the triangle entry T(r,k) */
    seq_t val = binomial_small(r, k);
    val *= binomial_small(r + 1, k);
    val *= binomial_small(2 * k, k);

    seq_t denom = (seq_t)(k + 1);
    denom *= (seq_t)(k + 1);
    val /= denom;

    return val;
}
