/*
 * A371400  Triangle read by rows: T(n, k) = binomial(k + n, k)*binomial(2*n -
 *          k, n).
 * https://oeis.org/A371400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) with r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2, T(r,k) = binomial(k+r, k) * binomial(2r-k, r)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 108, u64 n <= 358,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS entry defines A371400 as the triangle read by rows. */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrtl((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);

    /* Correct any possible off‑by‑one due to rounding */
    idx_t tri = r * (r + 1) / 2;
    while (tri > n) {
        r--;
        tri = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
        tri = r * (r + 1) / 2;
    }

    /* Calculate column k */
    idx_t k = n - tri;

    /* Compute the triangle entry */
    seq_t res = binomial(r + k, k);
    res *= binomial(2 * r - k, r);
    return res;
}
