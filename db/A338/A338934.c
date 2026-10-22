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
 * Helpers:   binomial, triangular_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/triangular_d.h"

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
