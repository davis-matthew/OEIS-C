/*
 * A382135  Square array read by antidiagonals: T(n,k) = S(n+k) - S(n) - S(k)
 *          - min(n,k), where S(k) = A000788(k-1).
 * https://oeis.org/A382135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, tabl
 * Formula:   a(n) = T(i,j) where the square array is read by antidiagonals:
 *            diagonal sum d = i + j, d ≥ 2; within each diagonal i runs from
 *            1 to d‑1. T(i,j) = C(i+j) – C(i) – C(j) – min(i,j), C(m) =
 *            Σ_{x=0}^{m-1} popcount(x) (cumulative binary weight)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   cum_weight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/cum_weight.h"

/* find diagonal d such that ((d-1)*d)/2 >= n  and ((d-2)*(d-1))/2 < n */
static inline idx_t diag_of(idx_t n)
{
    /* initial guess via quadratic formula */
    long double disc = sqrt((long double)1 + 8 * (long double)n);
    idx_t d = (idx_t)ceil((1 + disc) / 2);

    /* correct possible off‑by‑one errors */
    while (((__int128)d * (d - 1)) / 2 < n) d++;
    while (((__int128)(d - 1) * (d - 2)) / 2 >= n) d--;
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset is 1 */

    /* locate (i,j) from index n */
    idx_t d = diag_of(n);                               /* diagonal sum */
    idx_t prev = ((d - 1) * (d - 2)) / 2;               /* elements before this diagonal */
    idx_t pos = n - prev - 1;                           /* zero‑based position within diagonal */
    idx_t i = pos + 1;
    idx_t j = d - i;

    /* compute T(i,j) = C(i+j) - C(i) - C(j) - min(i,j) */
    seq_t sum_ij   = cum_weight(i + j);
    seq_t sum_i    = cum_weight(i);
    seq_t sum_j    = cum_weight(j);
    seq_t mn       = (i < j ? (seq_t)i : (seq_t)j);
    seq_t result   = sum_ij - sum_i - sum_j - mn;

    return result;
}
