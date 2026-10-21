/*
 * A253283  Triangle read by rows: coefficients of the partial fraction
 *          decomposition of [d^n/dx^n] (x/(1-x))^n/n!.
 * https://oeis.org/A253283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) (row index) *
 *            k = n - r*(r+1)/2 (column index) * T(r,k) = C(r,k) * C(r+k-1,
 *            k-1) * special cases: T(0,0)=1, T(r,0)=0 for r>0 * * The
 *            function enumerates the triangle read by rows: * [1] * [0,1] *
 *            [0,2,3] * [0,3,12,10] * and returns the n-th term...
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 130, u64 n <= 424,
 *            u128 n <= 1519, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    double d = sqrt((double)(8 * n + 1));
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Column index */
    idx_t t = r * (r + 1) / 2;
    idx_t k = n - t;

    /* Special cases */
    if (r == 0 && k == 0)
        return (seq_t)1;
    if (k == 0)
        return (seq_t)0;

    /* Compute T(r,k) = C(r,k) * C(r+k-1, k-1) */
    seq_t c1 = binomial(r, k);
    seq_t c2 = binomial(r + k - 1, k - 1);
    return c1 * c2;
}
