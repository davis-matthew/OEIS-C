/*
 * A348419  Triangular table read by rows: T(n,k) is the k-th entry of the
 *          main diagonal of the inverse Hilbert matrix of order n.
 * https://oeis.org/A348419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 40, u64 n <= 113, u128 n <= 394,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A348419: Triangular table read by rows.
 *  a(n) is the k‑th entry on the main diagonal of the inverse Hilbert matrix of
 *  order m, where (m,k) is obtained from the triangular indexing:
 *      m = smallest integer with m(m+1)/2 >= n,
 *      k = n - (m-1)m/2      (1 <= k <= m).
 *
 *  Formula for the diagonal entry:
 *      a(n) = (2k-1) * C(m+k-1, m-k)^2 * C(2k-2, k-1)^2
 *
 *  The function uses the utility binomial(idx_t n, idx_t k) to compute the 
 *  binomial coefficients.
 */

#include <math.h>              /* for sqrt */
#include <stdint.h>            /* for uint64_t */
#include <stdbool.h>           /* for bool */
#include "../utils/binomial.h" /* for binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the matrix order m that contains the n-th term   */
    idx_t m = 0;
    if (n > 0) {
        /* Initial approximation via double arithmetic */
        double d = sqrt((double)(8ULL * (uint64_t)n + 1ULL));
        m = (idx_t)((d - 1.0) / 2.0);   /* floor */
        /* Adjust up or down to satisfy m(m+1)/2 >= n */
        while (m * (m + 1) / 2 < n) ++m;
        while (m > 0 && (m - 1) * m / 2 >= n) --m;
    }

    /* diagonal index k within the m×m matrix   */
    idx_t prev_sum = (m - 1) * m / 2;
    idx_t k = n - prev_sum;   /* 1-based */

    /* Compute binomial coefficients   */
    seq_t b1 = binomial(m + k - 1, m - k);     /* C(m+k-1, m-k)   */
    seq_t b2 = binomial(2 * k - 2, k - 1);     /* C(2k-2, k-1)    */

    /* Compute the diagonal entry   */
    seq_t result = b1;
    result *= b1;                 /* square of b1   */
    result *= b2;                 /* multiply by b2   */
    result *= b2;                 /* square of b2   */
    result *= (2 * k - 1);        /* multiply by (2k-1) */

    return result;
}
