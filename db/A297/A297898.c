/*
 * A297898  Triangle read by rows, T(n, k) =
 *          (-1)^(n-k)*binomial(n,k)*hypergeom([k - n, n + 1], k + 1, 2), for
 *          n >= 0 and 0 <= k <= n.
 * https://oeis.org/A297898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) * where r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 (0 ≤ k ≤ r) * * T(r,k) = Σ_{j=0}^{r-k} binomial(r-k,j)
 *            * binomial(r+j,j) * * The implementation computes r by an
 *            integer loop, then evaluates the * sum term by term using the
 *            provided binomial helper. No memoisation * or glo...
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Find r such that (r+1)(r+2)/2 > n ≥ r(r+1)/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* index within the row */
    idx_t cum_before = r * (r + 1) / 2;
    idx_t k = n - cum_before;          /* 0 ≤ k ≤ r */

    /* Compute T(r,k) = Σ_{j=0}^{r-k} binomial(r-k,j) * binomial(r+j,j) */
    idx_t maxj = r - k;
    seq_t sum = 0;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t b1 = binomial(r - k, j);
        seq_t b2 = binomial(r + j, j);
        sum += b1 * b2;
    }
    return sum;
}
