/*
 * A338934  Square array T(i,j) = Sum_{k=0...min(i,j)} C(i,k)*C(j,k)*C(2*k,k)
 *          (i>=0,j>=0), read by antidiagonals.
 * https://oeis.org/A338934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the anti‑diagonal coordinates for
 *            the index n; T(i,j) = Σ_{k=0}^{min(i,j)} C(i,k) * C(j,k) *
 *            C(2k,k)
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Find highest integer d with d*(d+1)/2 <= n. */
static inline idx_t triangular_d(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Expand hi until the triangular number exceeds n.  Protect against
       overflow: if hi would overflow, set it to the maximum idx_t value. */
    while ((__int128)hi * (hi + 1) / 2 <= n) {
        if (hi > (idx_t)(-1) / 2) {
            hi = (idx_t)(-1);
            break;
        }
        hi <<= 1;
    }

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t d   = triangular_d(n);                  /* diagonal index */
    idx_t pos = n - d * (d + 1) / 2;              /* position on the diagonal */
    idx_t i   = pos;                               /* row index     */
    idx_t j   = d - pos;                           /* column index  */
    idx_t kmax = (i < j ? i : j);

    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term = binomial(i, k);              /* C(i,k)       */
        term *= binomial(j, k);                   /* * C(j,k)     */
        term *= binomial(2 * k, k);               /* * C(2k,k)    */
        sum += term;
    }
    return sum;
}
