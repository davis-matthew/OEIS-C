/*
 * A288515  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} ((1 + x^j)/(1 -
 *          x^j))^k.
 * https://oeis.org/A288515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is the element A(i,j) of the square array whose * entry
 *            A(i,j) is the coefficient of x^i in * \prod_{t>=1}
 *            ((1+x^t)/(1-x^t))^j . * The pair (i,j) is obtained by reading
 *            the array by * antidiagonals: n = i + j + choose(i+j,2), so that
 *            * i = n - j(j+1)/2, j = j. (See comments in the code.)
 * Range:     u8 n <= 38, u16 n <= 97, u32 n <= 311, u64 n <= 1058,
 *            u128 n <= 3792, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t tri = d * (d + 1) / 2;
    idx_t i = n - tri;        /* row index   (coefficient degree)    */
    idx_t j = d - i;          /* column index (exponent of the product) */

    if (i == 0) return (seq_t)1;          /* top row consists of ones   */
    if (j == 0) return (seq_t)0;          /* column 0 contributions are 0 after the constant term */

    /* We only need to compute coefficients up to degree i (<= 141 for n<=10000) */
    /* Local arrays are cheap because the size is small (≤ 200) */
    seq_t pol[200]   = {0};
    seq_t newP[200]  = {0};

    pol[0] = 1;                           /* start with the constant polynomial 1 */

    /* Multiply progressively by each factor ((1+x^t)/(1-x^t))^j     */
    for (idx_t t = 1; t <= i; ++t) {
        idx_t maxs = i / t;               /* maximal s with s*t <= i   */
        seq_t f[200] = {0};

        /* Build the factor polynomial f[s] for exponents s*t (s=0..maxs)  */
        for (idx_t s = 0; s <= maxs; ++s) {
            seq_t val = 0;
            idx_t limit_b = (s < j ? s : j);   /* b ranges 0..min(s,j) */
            for (idx_t b = 0; b <= limit_b; ++b) {
                seq_t t1 = binom(j, b);
                /* term2 = binom(j + (s - b) - 1, s - b) */
                idx_t m = s - b;
                seq_t t2 = binom(j + m - 1, m);
                val += t1 * t2;
            }
            f[s] = val;
        }

        /* Convolve current polynomial 'pol' with factor 'f',
           keeping only terms up to degree i.              */
        for (idx_t k = 0; k <= i; ++k) newP[k] = 0;

        for (idx_t n1 = 0; n1 <= i; ++n1) {
            if (pol[n1] == 0) continue;
            for (idx_t s = 0; s <= maxs; ++s) {
                idx_t pos = n1 + s * t;
                if (pos > i) break;
                newP[pos] += pol[n1] * f[s];
            }
        }

        /* Prepare for next iteration */
        for (idx_t k = 0; k <= i; ++k) pol[k] = newP[k];
    }

    return pol[i];
}
