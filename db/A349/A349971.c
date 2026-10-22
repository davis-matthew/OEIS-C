/*
 * A349971  Array read by ascending antidiagonals, A(n, k) =
 *          -(-n)^k*FallingFactorial(1/n, k) for n, k >= 1.
 * https://oeis.org/A349971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array A(n,k) along ascending
 *            antidiagonals (sum n+k increasing, with n decreasing within each
 *            diagonal). The array is defined by A(n,k) = ∏_{i=1}^{k-1} (n*i -
 *            1) (n,k ≥ 1), where the empty product (k=1) equals 1
 * Range:     u8 n <= 25, u16 n <= 41, u32 n <= 86, u64 n <= 184,
 *            u128 n <= 428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t m)
{
    /* Find the diagonal d (sum = n+k) such that the first d-1
       diagonals contain fewer than m elements, and d contains the
       m‑th element.  The number of elements up to diagonal d-1 is
       T(d-1) = (d-2)(d-1)/2, so we search for the minimal d with
       d(d-1)/2 >= m. */
    idx_t d = 2;
    idx_t cnt = 1;                       /* elements up to diagonal d   */
    while (cnt < m) {
        ++d;
        cnt = d * (d - 1) / 2;
    }
    /* Elements before this diagonal:
       T(d-1) = (d-2)(d-1)/2. */
    idx_t before = (d - 2) * (d - 1) / 2;
    /* Offset within the diagonal (0-based): */
    idx_t offset = m - before - 1;
    /* Row index (n) runs from d-1 down to 1: */
    idx_t n = (d - 1) - offset;
    /* Column index (k) = diagonal sum - n: */
    idx_t k = d - n;

    /* Compute A(n,k) = ∏_{i=1}^{k-1} (n*i - 1).  For k = 1 the
       product is the empty product and equals 1. */
    seq_t prod = 1;
    for (idx_t i = 1; i < k; ++i) {
        prod *= ((seq_t)n * (seq_t)i - 1);
    }
    return prod;
}
