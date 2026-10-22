/*
 * A335997  Triangle read by rows: T(n,k) = Product_{i=n-k+1..n} i! for 0 <= k
 *          <= n.
 * https://oeis.org/A335997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(m,k) where m = ⌊(√(8n+1) – 1)/2⌋, k = n – m(m+1)/2, and
 *            T(m,k) = ∏_{i=m–k+1}^{m} i! (product of the factorials)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find row index m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2 */
    idx_t m;
    {
        double d = sqrt((double)8 * (double)n + 1.0);
        m = (idx_t)((d - 1.0) * 0.5);
        /* Correct possible rounding errors */
        while (((unsigned __int128)(m + 1) * (m + 2) / 2) <= n)
            ++m;
        while (((unsigned __int128)m * (m + 1) / 2) > n)
            --m;
    }

    idx_t tri = (idx_t)(((unsigned __int128)m * (m + 1)) / 2);
    idx_t k = n - tri;

    /* Empty product when k = 0 */
    if (k == 0)
        return (seq_t)1;

    idx_t start = m - k + 1;          /* first i in the product */

    seq_t fact = (seq_t)1;
    /* Compute factorial(start‑1) */
    for (idx_t j = (idx_t)1; j < start; ++j)
        fact *= (seq_t)j;

    seq_t prod = (seq_t)1;
    for (idx_t i = start; i <= m; ++i)
    {
        fact *= (seq_t)i;   /* factorial(i) */
        prod *= fact;       /* multiply into result */
    }

    return prod;
}
