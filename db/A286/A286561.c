/*
 * A286561  Square array A(n,k): A(n,1) = 1, and for k > 1, A(n,k) = the
 *          highest exponent e such that k^e divides n, read by descending
 *          antidiagonals as A(1,1), A(1,2), A(2,1), etc.
 * https://oeis.org/A286561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = highest exponent e such that k^e divides n, where (n,k)
 *            is taken from the square array A(n,k) read by descending
 *            antidiagonals: (1,1), (1,2),(2,1), (1,3),(2,2),(3,1), …
 * Range:     u8 n <= 10585, u16 n <= 10585, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 136 terms through n = 9723 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute v_k(n): the largest e with k^e | n.  k>=1, n>=1. */
static seq_t v_power(seq_t n, seq_t k)
{
    /* k==1 by definition of the array */
    if (k == (seq_t)1)
        return (seq_t)1;

    seq_t e = (seq_t)0;
    while (n % k == (seq_t)0) {
        ++e;
        n /= k;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal number m such that T(m-1) < n <= T(m),
       where T(m) = m*(m+1)/2.  m and n are small (n <= 10000).      */
    idx_t m = 1;
    idx_t tri = 1; /* T(m) */
    while (tri < n) {
        ++m;
        tri += m;     /* T(m) = T(m-1) + m */
    }
    idx_t prev_tri = tri - m;          /* T(m-1) */
    idx_t pos = n - prev_tri;          /* position within this antidiagonal (1-based) */

    /* Row index (original n in A(n,k)) */
    seq_t row = (seq_t)pos;
    /* Column index k = m - pos + 1 */
    seq_t col = (seq_t)(m - pos + 1);

    return v_power(row, col);
}
