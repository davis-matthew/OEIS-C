/*
 * A294508  Regular triangular array read by rows: T(n,m) = pi(n*m) -
 *          pi(n)*pi(m) for n > 0 and 0 < m <= n.
 * https://oeis.org/A294508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = pi(r*m) - pi(r)*pi(m) where r and m are obtained by
 *            reading the regular triangular array by rows: a(1) = T(1,1) =
 *            pi(1) - pi(1)*pi(1) = 0. For a given index n ≥ 1: find the
 *            smallest r with r(r+1)/2 ≥ n, let m = n - (r-1)r/2, then a(n) =
 *            T(r,m)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is always ≥ 1 in use, but guard */

    /* Find the row r such that T(r-1) < n ≤ T(r) with T(k)=k(k+1)/2 */
    idx_t r = 1;
    idx_t tri = 1;                 /* r*(r+1)/2 for r=1 */
    while (tri < n) {
        ++r;
        /* Use idx_t to avoid overflow for the indices we handle */
        tri = r * (r + 1) / 2;
    }
    /* Compute column index m (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t m = n - prev_tri;        /* 1 ≤ m ≤ r */

    /* Evaluate T(r,m) = pi(r*m) – pi(r)*pi(m) */
    seq_t pi_r  = compute_pi(r);
    seq_t pi_m  = compute_pi(m);
    seq_t pi_rm = compute_pi(r * m);

    return pi_rm - pi_r * pi_m;
}
