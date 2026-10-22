/*
 * A346911  Triangle read by rows: T(n,k) is the number of k-dimensional
 *          simplices with vertices from the n-dimensional cross polytope; 0
 *          <= k < n.
 * https://oeis.org/A346911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) is the n‑th term of the triangle read by rows * where the
 *            triangle T has entries: * T(m,0) = 2*m * T(m,1) = 2*m^2 - m *
 *            T(m,k) = 2^(k+1) * C(m, k+1) for k > 1 * * For a given index n
 *            (1‑based) we find the unique row m and column k * such that
 *            (m-1)m/2 < n ≤ m(m+1)/2 and k = n - (m-1)m/2 - 1
 * Range:     u8 n <= 23, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* outside the offset, undefined, return 0 */

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t r   = 1;
    idx_t sum = 1;            /* r(r+1)/2 for current r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* column index (0‑based) within the row */
    idx_t prev = sum - r;              /* r(r-1)/2 */
    idx_t k    = n - prev - 1;

    /* compute the triangle entry */
    if (k == 0) {
        return (seq_t)2 * (seq_t)r;
    }
    if (k == 1) {
        return (seq_t)2 * (seq_t)r * (seq_t)r - (seq_t)r;
    }
    /* k >= 2 */
    seq_t b = binomial(r, k + 1);
    seq_t p = pow2(k + 1);
    return b * p;
}
