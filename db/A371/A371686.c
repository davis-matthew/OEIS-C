/*
 * A371686  Triangle read by rows: T(n, k) = e * binomial(n, k) * Gamma(k + 1,
 *          1).
 * https://oeis.org/A371686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where * T(row, col) = binomial(row, col) *
 *            Σ_{t=0}^{col} (col! / t!) * (row, col) are the row and column
 *            indices of the n-th term * in the triangle read by rows. * This
 *            uses the provided binomial() and fact_upto() utilities
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n) {
    /* Find the row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (_floor_isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t start = r * (r + 1) / 2;        /* first index of this row   */
    idx_t k = n - start;                  /* column within the row     */

    /* C(r,k) */
    seq_t bin = binomial(r, k);

    /* k! */
    seq_t factk = fact_upto(k);

    /* Σ_{t=0}^{k} (k! / t!)  computed iteratively */
    seq_t term = factk;   /* k!/0! = k!          */
    seq_t sum  = term;    /* start sum with t=0  */

    for (idx_t t = 1; t <= k; ++t) {
        term /= t;   /* k!/t! = (k!/(t-1)!)/t   */
        sum += term;
    }

    return bin * sum;
}
