/*
 * A364519  Square array read by ascending antidiagonals: T(n,k) = [x^(3*k)] (
 *          (1 + x)^(n+3)/(1 - x)^(n-3) )^k for n, k >= 0.
 * https://oeis.org/A364519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = read square array T(n,k) by ascending antidiagonals,
 *            where T(n,k) = [x^(3k)] ((1 + x)^(n+3)/(1 - x)^(n-3))^k computed
 *            by the summation T(n,k) = Sum_{j=0}^{3k} binom((n+3)k, j) *
 *            binom(nk - j - 1, 3k - j)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 40, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_general(int64_t n, idx_t k)
{
    if (k == 0)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - (int64_t)i + 1) / (seq_t)i;
    }
    return res;
}

static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 0)
        return (seq_t)1;                /* T(n,0)=1 for all n */
    idx_t mult1 = (n + 3) * k;          /* (n+3)k */
    idx_t thr = 3 * k;                  /* upper index in the sum */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= thr; ++j) {
        seq_t b1 = binom_general((int64_t)mult1, j);
        int64_t num = (int64_t)n * (int64_t)k - (int64_t)j - 1; /* nk - j - 1  */
        idx_t den = thr - j;                    /* 3k - j    */
        seq_t b2 = binom_general(num, den);
        sum += b1 * b2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Locate (i,j) that gives the nth antidiagonal entry.
       d = floor( (sqrt(8n+1) - 1) / 2 )  -> size of antidiagonal minus 1 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;
    idx_t prev = d * (d + 1) / 2;         /* elements before diagonal d      */
    idx_t offset = n - prev;              /* position inside diagonal d     */
    idx_t i = d - offset;                 /* row index n in the array       */
    idx_t j = offset;                     /* column index k in the array    */
    return compute_T(i, j);
}
