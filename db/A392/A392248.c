/*
 * A392248  Array read by antidiagonals: A(n, k) is the maximum number of
 *          whole unit squares under the polygonal path (i, h_k'(v_i)),
 *          associated with a k-dimensional balanced ballot path, n >= 1, k >=
 *          2.
 * https://oeis.org/A392248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let m = floor(k/2). For even k: A(k,n) = m^2 * n * (n-1) + n *
 *            (m-1) * m * (4*m + 1) / 3 For odd k: A(k,n) = (m+1) * m * n *
 *            (n-1) + n * (m+1) * m * (4*m - 1) / 3 The sequence a(n) is
 *            obtained by reading the table A(k,n) (rows k = 2,3,…) and
 *            columns n = 1,2,… along antidiagonals: a(1)=A(2,1), a(2...
 * Range:     u8 n <= 26, u16 n <= 693, u32 n <= 8441, u64 n <= 8441,
 *            u128 n <= 8441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static /* find the smallest m such that m*(m+1)/2 >= n */
idx_t
find_m(idx_t n)
{
    /* binary search using __int128 to avoid overflow */
    idx_t low = 1, high = 1;
    while (1) {
        __int128 val = (__int128)high * (high + 1) / 2;
        if (val >= n) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t
compute_A(idx_t k, idx_t n)
{
    idx_t m = k >> 1;          /* floor(k/2) */
    if ((k & 1) == 0) {        /* even k */
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t term1 = m_sq * ((seq_t)n * (n - 1));
        seq_t num = (seq_t)n *
                    (seq_t)(m - 1) * (seq_t)m *
                    (4 * (seq_t)m + 1);
        seq_t term2 = num / 3;
        return term1 + term2;
    } else {                   /* odd k */
        seq_t m1 = (seq_t)(m + 1);
        seq_t term1 = m1 * (seq_t)m *
                      ((seq_t)n * (n - 1));
        seq_t num = (seq_t)n *
                    m1 *
                    (seq_t)m *
                    (4 * (seq_t)m - 1);
        seq_t term2 = num / 3;
        return term1 + term2;
    }
}

seq_t
a(idx_t n)
{
    /* Find diagonal sum index S such that the cumulative count
       up to that diagonal ≥ n. Let m = S-2, so that each diagonal
       has size m and cumulative count = m(m+1)/2. */
    idx_t m = find_m(n);                       /* m >= 1 */
    idx_t prev = (m - 1) * m / 2;               /* cumulative before this diagonal */
    idx_t offset = n - prev - 1;               /* 0‑based offset within diagonal */
    idx_t k = 2 + offset;                      /* row index (row k) */
    idx_t col = m - offset;                    /* column index (column n) */
    return compute_A(k, col);
}
