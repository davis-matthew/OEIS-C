/*
 * A285722  Square array A(n,k) read by antidiagonals, A(n,n) = 0, otherwise,
 *          if n > k, A(n,k) = T(n-k,k), else A(n,k) = T(n,k-n), where T(n,k)
 *          is sequence A000027 considered as a two-dimensional table.
 * https://oeis.org/A285722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at position n in square array A(n,k), read by
 *            descending antidiagonals. Compute diagonal d with d*(d+1)/2 >=
 *            n, then position (i,j). If i=j return 0, else use Cantor pairing
 *            T(m,k) = (m+k-2)*(m+k-1)/2 + m with m = i-j, k = j (if i>j) or m
 *            = i, k = j-i (if i<j)
 * Range:     u8 n <= 299, u16 n <= 9517, u32 n <= 9517, u64 n <= 9517,
 *            u128 n <= 9517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that sum_{t=1}^{d-1} t < n <= sum_{t=1}^{d} t */
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++d;
        sum += d;
    }
    idx_t prev_sum = sum - d;          /* sum of 1..(d-1) */
    idx_t offset = n - prev_sum - 1;   /* 0‑based offset inside diagonal */

    idx_t i = offset + 1;              /* row index (1‑based) */
    idx_t j = d - offset;              /* column index (1‑based) */

    if (i == j) {
        return (seq_t)0;
    }

    idx_t m, k;
    if (i > j) {
        m = i - j;
        k = j;
    } else {
        m = i;
        k = j - i;
    }

    /* Cantor pairing function: T(m,k) = (m+k-2)*(m+k-1)/2 + m */
    seq_t m_t = (seq_t)m;
    seq_t k_t = (seq_t)k;
    seq_t sum_mk = m_t + k_t - 2;
    seq_t pair = (sum_mk * (m_t + k_t - 1)) / 2 + m_t;

    return pair;
}
