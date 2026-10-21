/*
 * A297477  Triangle read by rows: T(n, k) gives the coefficients of x^k of
 *          the characteristic polynomial P(n, x) of the n X n matrix M with
 *          entries M(i, j) = 1 if i = 1 or j = 1, -1 if i = j > 1, and 0
 *          otherwise. T(0, 0) := 0.
 * https://oeis.org/A297477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) is the (n‑th) element of the triangle T read row by row *
 *            where T(r,k) is the coefficient of x^k in the characteristic
 *            polynomial * of the r×r matrix M with entries M(i,j)=1 if i=0 or
 *            j=0, -1 if i==j>0, * and 0 otherwise. T(r,0)=(-1)^(r+1) r,
 *            T(r,1)=(-1)^(r+1) r (r-2), and * for k≥2 T(r,k)=(...
 * Range:     i8 n <= 38, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that n = r(r+1)/2 + k, 0≤k≤r. */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sum = mid * (mid + 1) / 2;
        if (sum <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;
    idx_t k = n - r * (r + 1) / 2;

    /* Even for r==0 the following works, but handle it explicitly for clarity. */
    if (r == 0)
        return (seq_t)0;

    seq_t rseq = (seq_t)r;

    /* k == 0 */
    if (k == 0) {
        seq_t sign = ((r + 1) & 1) ? -1 : 1;              /* (-1)^(r+1) */
        return sign * rseq;
    }

    /* k == 1 */
    if (k == 1) {
        seq_t sign = ((r + 1) & 1) ? -1 : 1;              /* (-1)^(r+1) */
        return sign * rseq * (rseq - (seq_t)2);
    }

    /* k >= 2 */
    seq_t sign = (r & 1) ? -1 : 1;                       /* (-1)^r */
    idx_t n_minus_2 = r - 2;
    seq_t bin1 = binomial(n_minus_2, k - 2);             /* binomial(r-2, k-2) */
    seq_t bin2 = binomial(n_minus_2, k);                 /* binomial(r-2, k)   */
    return sign * (bin1 - rseq * bin2);
}
