/*
 * A367024  Triangle read by rows, T(n, k) = [x^k] -hypergeom([-1/2, -n, -n],
 *          [1, 1], 4*x).
 * https://oeis.org/A367024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 31, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367024: a(n) = T(i,k) where i = floor((sqrt(8*n+1)-1)/2), 
 * k = n - i*(i+1)/2; T(i,k) = binomial(i,k)^2 * binomial(2*k,k) / (2*k-1) */
#include <stdint.h>

/* compute a(n) for OEIS A367024 */
seq_t a(idx_t n)
{
    /* find the row i such that i(i+1)/2 <= n < (i+1)(i+2)/2 */
    idx_t i = 0;
    for (;; ++i) {
        __int128 next = (__int128)(i + 1) * (i + 2) / 2;
        if (next > n)
            break;
    }
    /* column k within that row */
    idx_t k = n - i * (i + 1) / 2;

    /* binomial(i,k) */
    seq_t bin_nk = 1;
    for (idx_t j = 1; j <= k; ++j) {
        bin_nk = bin_nk * (seq_t)(i - k + j) / (seq_t)j;
    }

    /* binomial(2*k,k) */
    seq_t bin_2k_k = 1;
    for (idx_t j = 1; j <= k; ++j) {
        bin_2k_k = bin_2k_k * (seq_t)(k + j) / (seq_t)j;
    }

    /* compute the term */
    seq_t res = bin_nk * bin_nk;
    res = res * bin_2k_k;
    seq_t denom = (seq_t)(2 * k) - 1;
    res = res / denom;
    return res;
}
