/*
 * A382799  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = n! * k! * [x^n * y^k] 1 / (1 - log(1-x)
 *          * log(1-y))^2.
 * https://oeis.org/A382799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0}^{min(i,j)} k! * (k+1)! * |Stirling1(i,k)| *
 *            |Stirling1(j,k)| where (i,j) are the antidiagonal coordinates of
 *            n: let s be the largest integer with s*(s+1)/2 <= n, let d = n -
 *            s*(s+1)/2, i = s - d, j = d
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes Stirling numbers of the first kind
   and factorials up to a fixed bound (200) which covers all indices
   needed for n up to about 10000.  */

#include <stdbool.h>
#include <stdint.h>

#define MAX_N 200   /* sufficient for n <= 10000 */

static bool _init_done = false;
static seq_t _st1[MAX_N + 1][MAX_N + 1];
static seq_t _fact[MAX_N + 1];

static void _precompute(void) {
    if (_init_done) return;
    _st1[0][0] = 1;
    _fact[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i) {
        _fact[i] = _fact[i-1] * i;
    }
    for (idx_t n = 1; n <= MAX_N; ++n) {
        _st1[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            /* st1[n][k] = st1[n-1][k-1] + (n-1)*st1[n-1][k] */
            seq_t term = (seq_t)(n - 1) * _st1[n-1][k];
            _st1[n][k] = _st1[n-1][k-1] + term;
        }
    }
    _init_done = true;
}

seq_t a(idx_t n) {
    _precompute();

    /* Find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s * (s + 1) / 2) <= n) {
        s++;
    }
    s--;  /* now s satisfies the condition */

    idx_t prevCnt = s * (s + 1) / 2;
    idx_t d = n - prevCnt;
    idx_t i = s - d;
    idx_t j = d;

    /* Compute the entry A(i,j) */
    idx_t min_ij = (i < j) ? i : j;
    seq_t result = 0;
    for (idx_t k = 0; k <= min_ij; ++k) {
        seq_t term = _fact[k] * _fact[k + 1];
        term = term * _st1[i][k];
        term = term * _st1[j][k];
        result = result + term;
    }
    return result;
}
