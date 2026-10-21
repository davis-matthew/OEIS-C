/*
 * A282628  Triangle T(n, k) read by rows: row n gives for n >= 0 the
 *          coefficients of the exponential numerator polynomial used for the
 *          exponential generating function of {Sum_{j=1..m} (1 +
 *          2*j)^n}_{m>=0}.
 * https://oeis.org/A282628
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) defined as *
 *            T(n,0) = 1 * T(n,k) = Σ_{j=1..k} (-1)^{k-j} * binomial(k-1, j-1)
 *            * (2*j+1)^n (for k>0) * where 0 ≤ k ≤ n+1 and n ≥ 0. A(-1,0)=1
 *            is used only to make a triangle
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions */
#include <math.h>                 /* sqrt */
#include "../utils/binomial.h"    /* binomial(idx_t,idx_t) -> seq_t */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t,idx_t) -> seq_t */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t,seq_t) -> seq_t */

/* Return the nth term of OEIS A282628, with a(-1)=1. */
seq_t a(idx_t n)
{
    /* Handle the special -1 offset */
    if (n == -1) return 1;

    /* For n ≥ 0 map the flat index to (row, col) in the triangle T. */
    idx_t m = (idx_t)(n + 1);               /* shift the index so that the first term is m=0 */

    /* Find row r such that (r+2)(r+3)/2 > m.
     * Solve t(t+1)/2 > m where t = r+2.  Use a double approximation
     * and then correct by integer arithmetic. */
    double dm = (double)m;
    idx_t t = (idx_t)((sqrt(1.0 + 8.0 * dm) - 1.0) / 2.0);
    while ((t * (t + 1)) / 2 <= m) t++;        /* guarantee t(t+1)/2 > m */
    /* t = r+2  -->  r = t-2  */
    idx_t r = t - 2;

    /* The number of terms up to the previous row (r-1) is ((r+1)(r+2))/2. */
    idx_t prev_t = t - 1;                      /* r+1 */
    idx_t prev_S = (prev_t * (prev_t + 1)) / 2;
    idx_t k = m - prev_S;                      /* column index in the current row */

    /* Compute T(r,k) using the explicit summation formula. */
    if (k == 0) return 1;                     /* T(n,0) = 1 for all n */

    seq_t pos = 0;     /* sum of positive terms */
    seq_t neg = 0;     /* sum of negative terms (will be subtracted) */

    for (idx_t j = 1; j <= k; ++j) {
        seq_t bin = binomial(k - 1, j - 1);                      /* C(k-1, j-1) */
        seq_t powv = seq_pow((seq_t)(2 * j + 1), (idx_t)r);      /* (2*j+1)^r */
        seq_t term = seq_mul(bin, powv);                         /* binomial * power */
        if ((k - j) & 1) {
            neg = neg + term;
        } else {
            pos = pos + term;
        }
    }

    /* pos ≥ neg for all admissible (r,k), so subtraction is safe */
    return pos - neg;
}
