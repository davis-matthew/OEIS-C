/*
 * A307214  Triangle read by rows: number of forests of (k+1) ordered trees
 *          with 2(n-k) edges having root of even degree and nonroot nodes of
 *          outdegree 0 or 2.
 * https://oeis.org/A307214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2 T(n,m) = 1 for m=n T(n,m) = \sum_{k=1}^{n-m} k/(n-m) *
 *            C(2n-2m, n-m-k) * C(k+m,k)
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

static seq_t triangle_T(idx_t n, idx_t m)
{
    if (n == m)
        return 1;                       /* last element in a row */

    /* n > m: number of terms to sum = n-m */
    idx_t denom = n - m;
    seq_t sum_num = 0;

    for (idx_t k = 1; k <= denom; ++k)
    {
        seq_t a = binomial(2 * n - 2 * m, denom - k);
        seq_t b = binomial(k + m, k);
        seq_t term = a * b;          /* seq_t multiplication */
        term *= k;
        sum_num += term;
    }
    /* divide by denom (exact division) */
    return sum_num / (seq_t)denom;
}

seq_t a(idx_t n)
{
    /* Determine row r and column c such that
       n = r(r+1)/2 + c , 0 <= c <= r */
    idx_t r;
    if (n == 0)
        r = 0;
    else
    {
        /* crude approximation of r */
        r = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
        /* correct possible off-by-one due to rounding */
        while ((r + 1) * (r + 2) / 2 <= n)
            ++r;
        while (r * (r + 1) / 2 > n)
            --r;
    }

    idx_t cumPrev = r * (r + 1) / 2;
    idx_t c = n - cumPrev;

    return triangle_T(r, c);
}
