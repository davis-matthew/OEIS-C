/*
 * A391403  Triangle read by rows: T(n,k) is k-th entry of the gamma-vector of
 *          the n-dimensional cyclohedron, 0 <= k <= floor(n/2).
 * https://oeis.org/A391403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   The OEIS sequence A391403 is obtained by reading the triangle
 *            T(n,k) by rows, where T(n,k) = binomial(n, 2*k) * binomial(2*k,
 *            k) for 0 ≤ k ≤ ⌊n/2⌋. a(0) = T(0,0), a(1) = T(1,0), a(2) =
 *            T(2,0), a(3) = T(2,1), etc
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 162, u64 n <= 541,
 *            u128 n <= 1876, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t row_and_pos(idx_t idx, idx_t *row, idx_t *pos)
{
    idx_t r = 0;
    idx_t cum = 0;  /* number of terms before current row */
    while (1) {
        idx_t len = (r / 2) + 1;  /* floor(r/2)+1 */
        if (cum + len > idx) {
            *row = r;
            *pos = idx - cum;
            return 0;
        }
        cum += len;
        ++r;
    }
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_and_pos(n, &r, &k);
    seq_t c1 = binomial(r, 2 * k);
    seq_t c2 = binomial(2 * k, k);
    return c1 * c2;
}
