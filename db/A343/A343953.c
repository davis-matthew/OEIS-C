/*
 * A343953  Square array T(n,k), n>=1, k>=0, read by antidiagonals, where row
 *          n is the expansion of x*(1+x^n)/((1-x)^2*(1-x^n)).
 * https://oeis.org/A343953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the antidiagonal reading of the square
 *            array T(r, k) where row r (r>=2) is the expansion of
 *            x*(1+x^(r-1))/((1-x)^2*(1-x^(r-1)))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t T_row_k(idx_t row, idx_t k)
{
    /* row >= 2, k >= 0 */
    if (k == 0) return 0;
    idx_t n = row - 1;               /* n >= 1 */
    idx_t a = (k - 1) / n;           /* floor((k-1)/n)   */
    unsigned long long term1 = (unsigned long long)(a + 1) * k;
    unsigned long long term1_sub = (unsigned long long)n * a * (a + 1) / 2;
    unsigned long long S1 = term1 - term1_sub;

    unsigned long long S2 = 0;
    if (k > n) {
        idx_t a2 = (k - n - 1) / n; /* floor((k-n-1)/n) */
        unsigned long long term2 = (unsigned long long)(a2 + 1) * (k - n);
        unsigned long long term2_sub = (unsigned long long)n * a2 * (a2 + 1) / 2;
        S2 = term2 - term2_sub;
    }
    return (seq_t)(S1 + S2);
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that (d-1)*d/2 > n with d >= 2 */
    idx_t d = 2;
    unsigned __int128 tri = (unsigned __int128)(d - 1) * d / 2;
    while (tri <= n) {
        ++d;
        tri = (unsigned __int128)(d - 1) * d / 2;
    }

    /* Previous diagonal count */
    unsigned __int128 prev_tri = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t prev = (idx_t)prev_tri;

    idx_t offset = n - prev;               /* 0 <= offset < d-1 */
    idx_t row   = d - offset;              /* row >= 2 */
    idx_t k     = offset;                  /* column index */

    return T_row_k(row, k);
}
