/*
 * A341867  Square array read by downward antidiagonals: T(m,n) = Sum_{i=0..m,
 *          j=0..n} binomial(m,i)*binomial(n,j)*binomial(i+j,i).
 * https://oeis.org/A341867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where (m,n) are the coordinates on the downward
 *            antidiagonal * of the square array T with entries * T(m,n) =
 *            Σ_{i=0..m} Σ_{j=0..n} binomial(m,i) * binomial(n,j) *
 *            binomial(i+j,i) * * The index n is mapped to (m,n) by * d =
 *            floor((√(8n+1)-1)/2) (diagonal number) * p = n - d*(d+1)/2 (...
 * Range:     u8 n <= 28, u16 n <= 81, u32 n <= 261, u64 n <= 961,
 *            u128 n <= 3524, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"      /* provides seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find diagonal index d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    /* Since n ≤ 10 000, d will be < 150; a plain linear search is trivial. */
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    idx_t pos = n - d * (d + 1) / 2;   /* position on diagonal d   */
    idx_t m   = pos;                  /* row index                 */
    idx_t k   = d - pos;              /* column index              */

    seq_t result = (seq_t)0;

    for (idx_t i = 0; i <= m; ++i) {
        seq_t bin_m_i = binomial(m, i);
        for (idx_t j = 0; j <= k; ++j) {
            seq_t bin_k_j   = binomial(k, j);
            seq_t bin_ij_i  = binomial(i + j, i);
            result += bin_m_i * bin_k_j * bin_ij_i;
        }
    }

    return result;
}
