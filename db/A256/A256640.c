/*
 * A256640  Triangle read by rows: T(n,k) = Sum_{i=n-k..n}
 *          C(k-1,n-i)*C(i,n-k)*C(2*i,i)/(i+1).
 * https://oeis.org/A256640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(n,k) where * T(n,k) =
 *            Σ_{i=n-k..n} C(k-1,n-i) * C(i,n-k) * C(2i,i)/(i+1) * (k = 0..n)
 *            with offset 0
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utilities provide the binomial coefficient function:
 *     seq_t binomial(idx_t n, idx_t k);
 */
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/binomial.h"

static inline seq_t catalan(idx_t m)
{
    seq_t c = binomial(2 * m, m);
    return c / ((seq_t)m + 1);
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);          /* initial guess  */
    /* correct rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t base = r * (r + 1) / 2;   /* number of terms before row r   */
    idx_t k = n - base;              /* column within the row           */

    if (k == 0)                      /* first column = Catalan(r)      */
        return catalan(r);

    seq_t sum = 0;
    idx_t i_start = r - k;          /* lower bound of the sum          */
    for (idx_t i = i_start; i <= r; ++i) {
        seq_t term = binomial(k - 1, r - i);
        term *= binomial(i, r - k);
        term *= catalan(i);
        sum += term;
    }
    return sum;
}
