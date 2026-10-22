/*
 * A332352  Triangle read by rows: T(m,n) = Sum_{-m<i<m, -n<j<n, gcd{i,j}=2}
 *          (m-|i|)*(n-|j|), m >= n >= 1.
 * https://oeis.org/A332352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = Sum_{-m<i<m,-n<j<n, gcd(|i|,|j|)=2} (m-|i|)*(n-|j|)
 * Range:     u8 n <= 26, u16 n <= 350, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the triangle read by rows of T(m,n). */

#include "../utils/gcd.h"

/* Compute the triangle entry T(m,n).  m >= n >= 1. */
static seq_t compute_T(idx_t m, idx_t n)
{
    seq_t total = 0;
    for (idx_t a = 0; a < m; ++a)
    {
        seq_t mul_a = (a == 0) ? 1 : 2;
        for (idx_t b = 0; b < n; ++b)
        {
            seq_t mul_b = (b == 0) ? 1 : 2;
            if (gcd((seq_t)a, (seq_t)b) == 2)
            {
                seq_t term = mul_a * mul_b *
                             (seq_t)(m - a) * (seq_t)(n - b);
                total += term;
            }
        }
    }
    return total;
}

/* a(n): sequence read by rows of the triangle T(m,n). */
seq_t a(idx_t n)
{
    /* find row index m such that m(m+1)/2 >= n */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n)
        ++m;
    /* previous rows total number of terms */
    idx_t prev = (m - 1) * m / 2;
    /* position within the row (1‑based) */
    idx_t pos = n - prev;
    return compute_T(m, pos);
}
