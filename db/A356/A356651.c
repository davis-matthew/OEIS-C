/*
 * A356651  Triangle read by rows. T(n, k) = [x^k](0^n + 4^n * ((1 - x)^(-1/2)
 *          - 1)).
 * https://oeis.org/A356651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(n,k) where * T(n,0) = 0^n *
 *            T(n,n) = binomial(2n,n) * T(n,k) = 4^(n-k) * T(k,k) for 0 < k <
 *            n * The sequence a(n) lists all entries T(r,j) row‑by‑row. * For
 *            a given global index n we find the row r and column j such that
 *            * n = r(r+1)/2 + j , 0 <= j <= r * and retu...
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find row r and column j for global index n */
    idx_t r = 0;
    idx_t cum = 0;            /* cumulative length of rows 0 .. r-1      */
    while (cum + r + 1 <= n)   /* row r has r+1 elements                  */
    {
        cum += r + 1;
        r += 1;
    }
    idx_t j = n - cum;        /* column within row r                    */

    /* Row 0 is special: only one element which is 1        */
    if (r == 0)
        return (seq_t)1;

    /* Column 0 of any row n>0 is 0                         */
    if (j == 0)
        return (seq_t)0;

    /* For general positions: T(r,j) = 4^(r-j) * binomial(2j,j)   */
    seq_t top = binomial(2 * j, j);   /* C(2j,j)                         */
    if (j == r)                       /* diagonal entry T(r,r)            */
        return top;

    seq_t power = pow4(r - j);        /* 4^(r-j)                         */
    return power * top;
}
